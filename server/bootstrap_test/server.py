import os
import time
import json
import tornado.ioloop
import tornado.web
import tornado.websocket
from tornado.ioloop import PeriodicCallback
from tornado.options import define, options, parse_command_line

define("port", default = 8080, help = "run on the given port", type = int)

class RecipeModalHandler(tornado.web.RequestHandler):

    def get(self):
        self.render('index.html')

class IndexHandler(tornado.web.RequestHandler):
    @tornado.web.asynchronous
    def get(self):
        f = open("recipeData.json", "r", encoding='utf-8')
        recipeDataJson = json.loads(f.read())
        f.close()

        recipeNmaeList = []
        for i in range(len(recipeDataJson)) :
            recipeNmaeList.append((recipeDataJson[i]["recipeName"]))
        titles = recipeNmaeList
        self.render("index.html", titles=titles)

class SendWebSocket(tornado.websocket.WebSocketHandler):
    #on_message -> receive data
    #write_message -> send data

    def open(self):
        self.i = 0
        self.callback = PeriodicCallback(self._send_message, 400) #遅延用コールバック
        self.callback.start()
        print ("WebSocket opened")

    #クライアントからメッセージが送られてくると呼び出す
    def on_message(self, message):
        print (message)

        #リクエストを受けたレシピデータを送信
        if ('requestRecipeData' in str(message)) :
            recipeName = str(message).replace('requestRecipeData', '')
            f = open("recipeData.json", "r", encoding='utf-8')
            recipeDataJson = json.loads(f.read())
            f.close()
            for i in range(len(recipeDataJson)) :
                if (recipeDataJson[i]["recipeName"] == recipeName) :
                    break
            print (json.dumps(recipeDataJson[i], sort_keys=True, indent=2, ensure_ascii=False))
            self.write_message(recipeDataJson[i])

        #レシピ追加のボタンが押された時に空のレシピデータを送信
        elif ('addNewRecipe' in str(message)) :
            newRecipeDataJson = {
                    "recipeName" : "",
                    "recipeId" : 0,
                    "actionDataIf" : "-",
                    "actionDataThen" : "-",
                    "valueIf" : [""],
                    "valueThen" : [""]
                }
            self.write_message(newRecipeDataJson)

        #レシピ実行ボタンが押された時
        elif ('actionRecipe' in str(message)) :
            actionRecipeId = str(message).replace('actionRecipe', '')
            cmd = "mosquitto_pub -h 192.168.1.4 -t \"/client/actionRecipe\" -m \"" + actionRecipeId + "\""
            print (cmd)
            os.system(cmd)

        #レシピが変更・追加された時にレシピデータを外部ファイルrecipeData.jsonに出力
        elif ('{' in str(message)) :
            newRecipeData = json.loads(message)
            print (json.dumps(newRecipeData, sort_keys=True, indent=2, ensure_ascii=False))
            f = open("recipeData.json", "r", encoding='utf-8')
            recipeDataJson = json.loads(f.read())
            f.close()

            #新規レシピをrecipeData.jsonに出力
            if (newRecipeData["recipeId"] == 0) :
                print(0)
                try:
                    newRecipeData["recipeId"] = recipeDataJson[len(recipeDataJson)-1]["recipeId"] + 1
                except:
                    newRecipeData["recipeId"] = 1
                newRecipeDataList = recipeDataJson
                newRecipeDataList.append(newRecipeData)
                print (json.dumps(newRecipeData, sort_keys=True, indent=2, ensure_ascii=False))
                with open('recipeData.json', 'w') as f:
                    json.dump(newRecipeDataList, f, sort_keys=True, indent=2, ensure_ascii=False)

            #編集したレシピをrecipeData.jsonに出力
            elif (newRecipeData["recipeId"] <= int(recipeDataJson[len(recipeDataJson)-1]["recipeId"])) :
                print(1)
                newRecipeDataList = []
                for i in range(len(recipeDataJson)) :
                    if (newRecipeData["recipeId"] == recipeDataJson[i]["recipeId"]) :
                        newRecipeDataList.append(newRecipeData)
                    else :
                        newRecipeDataList.append(recipeDataJson[i])
                with open('recipeData.json', 'w') as f :
                    json.dump(newRecipeDataList, f, sort_keys=True, indent=2, ensure_ascii=False)

            cmd = "mosquitto_pub -h 192.168.1.4 -t \"/client/recipeData\" -m \"change\""
            print (cmd)
            os.system(cmd)

        #レシピの削除
        elif ('deleteRecipe' in str(message)) :
            deleteRecipeId = int(str(message).replace('deleteRecipe', ''))
            newRecipeDataList = []
            f = open("recipeData.json", "r", encoding='utf-8')
            recipeDataJson = json.loads(f.read())
            f.close()
            for i in range(len(recipeDataJson)) :
                if (deleteRecipeId != recipeDataJson[i]["recipeId"]) :
                    newRecipeDataList.append(recipeDataJson[i])
            with open('recipeData.json', 'w') as f:
                json.dump(newRecipeDataList, f, sort_keys=True, indent=2, ensure_ascii=False)
            cmd = "mosquitto_pub -h 192.168.1.4 -t \"/client/recipeData\" -m \"change\""
            print (cmd)
            os.system(cmd)


        #ネットワークリモコンのリクエストをMQTTブローカにpubを送信
        else :
            cmd = "mosquitto_pub -h 192.168.1.4 -t \"/client/tvIr\" -m \"" + message + "\""
            print (cmd)
            os.system(cmd)

    #コールバックスタートで呼び出し開始
    def _send_message(self):
        self.i += 1
        #self.write_message(str(self.i))

    #ページが閉じ，コネクションが切断された場合に呼び出す
    def on_close(self):
        self.callback.stop()
        print ("WebSocket closed")

app = tornado.web.Application([
    (r"/", IndexHandler),
    (r"/ws", SendWebSocket),
    ],
    static_path=os.path.join(os.getcwd(),  "bootstrap"),
)

if __name__ == "__main__":
    parse_command_line()
    app.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()


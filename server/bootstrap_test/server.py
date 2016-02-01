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
        titles = ["1 Item1", "2 Item2", "3 Item3", "4 Item4", "5 Item5", "6 Item6"]
        self.render("index.html", titles=titles)

class SendWebSocket(tornado.websocket.WebSocketHandler):
    #on_message -> receive data
    #write_message -> send data

    data = [{'recipeName' : 'recipeName1', 'thisData' : 'thisData1', 'thatData' : 'thatData1'}, {'recipeName' : 'recipeName2', 'thisData' : 'thisData2', 'thatData' : 'thatData2'}]


    def open(self):
        self.i = 0
        self.callback = PeriodicCallback(self._send_message, 400) #遅延用コールバック
        self.callback.start()
        print ("WebSocket opened")

    #クライアントからメッセージが送られてくると呼び出す
    def on_message(self, message):
        print (message)
        if ('requestMessage' in str(message)) :
            recipeData = {
                "recipeNum" : message[14],
                "thisData" : "人感センサが反応",
                "thatData" : "モータを回す"
            }
            self.write_message(recipeData)
        elif ('{' in str(message)) :
            newRecipeData = json.loads(message)
            print (json.dumps(newRecipeData, sort_keys=True, indent=2, ensure_ascii=False))
        else :
            cmd = "mosquitto_pub -h 192.168.1.32 -t \"test\" -m \"" + message + "\""
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


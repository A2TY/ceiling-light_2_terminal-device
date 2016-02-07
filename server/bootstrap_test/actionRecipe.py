# -*- coding: utf-8 -*-
import json
import paho.mqtt.client as mqtt

host = '192.168.1.4'
port = 1883
topic = 'test'

recipeDataList = {
    "recipeName" : "",
    "recipeId" : 0,
    "actionDataIf" : "-",
    "actionDataThen" : "-"
}

def transfórmTopicName(recipeDataJson):
    global recipeDataList
    topicList = [['人感センサが反応', '温度センサが反応', '湿度センサが反応', '加速度センサが反応', 'スイッチが押される', 'テレビをON', 'モータを回す', 'LEDを点灯させる', '音を鳴らす'], ['PIRsensor', 'temperature', 'humidity', 'acceleration', 'switch', 'tvIr', 'rotateMotor', 'onLED', 'ringSpeaker']]

    for i in range(len(recipeDataJson)):
        for j in range(len(topicList[0])):
            if (recipeDataJson[i]["actionDataIf"] == topicList[0][j]):
                recipeDataJson[i]["actionDataIf"] = topicList[1][j]
            if (recipeDataJson[i]["actionDataThen"] == topicList[0][j]):
                recipeDataJson[i]["actionDataThen"] = topicList[1][j]
    print (json.dumps(recipeDataJson, sort_keys=True, indent=2, ensure_ascii=False))
    recipeDataList = recipeDataJson.copy()

    return recipeDataList


def openRecipeDataFile():
    f = open("recipeData.json", "r", encoding='utf-8')
    recipeDataJson = json.loads(f.read())
    f.close()

    return recipeDataJson


def on_connect(client, userdata, flags, respons_code):
    print('status {0}'.format(respons_code))

    client.subscribe('/client/#')


def on_message(client, userdata, msg):
    global recipeDataList
    #print ("global test" + json.dumps(recipeDataList, sort_keys=True, indent=2, ensure_ascii=False))

    if ('recipeData' in str(msg.topic)):
        print ('recipeData')
        recipeDataList = transfórmTopicName(openRecipeDataFile())
    elif ('actionRecipe' in str(msg.topic)):
        print ('actionRecipe')
        for i in range(len(recipeDataList)):
            if (recipeDataList[i]["recipeId"] == int(msg.payload)):
                client.publish('/server/' + recipeDataList[i]["actionDataThen"], 'actionRecipeTest')
                break
    else :
        for i in range(len(recipeDataList)):
            if (recipeDataList[i]["actionDataIf"] in str(msg.topic)):
                client.publish('/server/' + recipeDataList[i]["actionDataThen"], 'てすと')
                print ('other')


    #print(msg.topic + ' ' + str(msg.payload.decode('utf-8')))


if __name__ == '__main__':
    transfórmTopicName(openRecipeDataFile())

    print (recipeDataList)

    # Publisherと同様に v3.1.1を利用
    client = mqtt.Client(protocol=mqtt.MQTTv311)

    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(host, port=port, keepalive=60)

    # 待ち受け状態にする
    client.loop_forever()



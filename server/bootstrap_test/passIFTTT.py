# -*- coding: utf-8 -*-
import re
import json
import requests
import paho.mqtt.client as mqtt

host = '___ADDRESS___'
port = ___PORT___


#Subscribeするtopicを設定
def on_connect(client, userdata, flags, respons_code):
    print('status {0}'.format(respons_code))

    client.subscribe('/server/IFTTT/#')   #クライアントからPubされたメッセージを全てSubする

#メッセージをSubしたらIFTTTに送信
def on_message(client, userdata, msg):
    global recipeDataList
    deletePattern = re.compile('[\[\]\'\ ]')

    messageList = str(msg.payload).lstrip("b'").rstrip("'").split(",")
    print(messageList)
    requests.post("https://maker.ifttt.com/trigger/" + str(messageList[0]) + "/with/key/" + str(messageList[1]))


if __name__ == '__main__':
    # Publisherと同様に v3.1.1を利用
    client = mqtt.Client(protocol=mqtt.MQTTv311)

    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(host, port=port, keepalive=60)

    # 待ち受け状態にする
    client.loop_forever()

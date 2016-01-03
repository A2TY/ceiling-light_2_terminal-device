# -*- coding: utf-8 -*-

import os
import tweepy


CONSUMER_KEY='___CONSUMER_KEY___'
CONSUMER_SECRET='___CONSUMER_SECRET___'
ACCESS_TOKEN_KEY='___ACCESS_TOKEN_KEY___'
ACCESS_TOKEN_SECRET='___ACCESS_TOKEN_SECRET___'

auth = tweepy.OAuthHandler(CONSUMER_KEY, CONSUMER_SECRET)
auth.set_access_token(ACCESS_TOKEN_KEY, ACCESS_TOKEN_SECRET)

#東上線遅延情報の取得（Twitterより）
class ScrapingTrainTweet():
    #前回のツイートと変化があったかを調べる
    def execute(self, api):
        tweetTextTrain = self.get_tweetTextTrain(api)
        f = open("tweetTextTrain.txt", "r", encoding='utf-8')       #前回のツイートを読み込む
        line = f.read()
        f.close()
        if not tweetTextTrain.text in line:
            f = open("tweetTextTrain.txt", "w", encoding='utf-8')   #前回のツイートを最新のものに更新
            f.write(tweetTextTrain.text)
            f.close()
            if "情報はありません" in tweetTextTrain.text:
                sendCommandTrainColor = "green"                     #平常通りの運行の時
                self.sendCommandMQTT(sendCommandTrainColor)
            else:
                sendCommandTrainColor = "red"                       #運行に遅延がある時
                self.sendCommandMQTT(sendCommandTrainColor)
        else:
            sendCommandTrainColor = "black"                         #前回のツイートと変化がなかった時
            self.sendCommandMQTT(sendCommandTrainColor)

    #東武東上線運行情報【非公式】（@train_tbtoj）から最新のツイートを1件取得
    def get_tweetTextTrain(api):
        tweetTextTrain = api.user_timeline(id=57332729, count=1)[0]
        return tweetTextTrain

    #MQTTサーバに電車の運行情報をpub
    def sendCommandMQTT(sendCommandTrainColor):
        cmd = "mosquitto_pub -h serverAddress -t \"device/device1/trainColor\" -m \"" + sendCommandTrainColor + "\""
        print (cmd)
        os.system(cmd)


#東京都の天気予報を取得（Twitterより）
class ScrapingWetherTweet():
    #取得したツイートに雨または雪のワードを探索
    def execute(self, api):
        tweetTextWether = self.get_tweetTextWether(api)
        if "雨" in tweetTextWether or "雪" in tweetTextWether:
            sendCommandWetherColor = "blue"
            self.sendCommandMQTT(sendCommandWetherColor)

    #東京都の天気予報（@tokyo_wf）から最新のツイートを1件取得
    def get_tweetTextWether(api):
        tweetTextWether = api.user_timeline(id=395441059, count=1)[0].text
        return tweetTextWether

    #MQTTサーバに天気情報をpub
    def sendCommandMQTT(sendCommandWetherColor):
        cmd = "mosquitto_pub -h serverAddress -t \"device/device1/wetherColor\" -m \"" + sendCommandWetherColor + "\""
        print (cmd)
        os.system(cmd)

api = tweepy.API(auth)
ScrapingTrainTweet.execute(ScrapingTrainTweet, api)
ScrapingWetherTweet.execute(ScrapingWetherTweet, api)

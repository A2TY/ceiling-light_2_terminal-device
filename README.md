# module-IoT

## 概要
このプロジェクトは，シーリングライトの電子端末化を行うものである．mosquittoサーバとしてRaspberry Piを使用し，クライアントにESP-WROOM-02を用いてMQTT通信により連携を行う．主な機能は，ネットワークリモコンおよび情報通知である．

リモコンサイトおよびレシピ管理サイトはBootstrapテーマ「Honoka」を使用しています．
[http://honokak.osaka/](http://honokak.osaka/)


## 機能
### ネットワークリモコン
スマートフォン・PCからWebSocketを用いて情報を送り，シーリングライトから赤外線LEDを用いてTVやエアコンに送信する．

### 情報通知
Twitterから情報を取得し，フルカラーLEDの点灯の違いによって情報を通知する．

**通知内容**
* 天気　　　　　　　tenki.jp （ 東京 ）　@tenkijp_tokyo
* 電車の遅延情報　　東武東上線運行情報【非公式】　@train_tbtoj

### リアル版IFTTT
あらゆるサービスを連携させるWebサービスIFTTTを現実の世界で行えるようにする．


## 使用した環境および言語
__Raspberry Pi 2 Model B__

* OS
	* RASPBIAN JESSIE（Kernel version:4.1）

* WebSocket
	* tornado

* MQTT
	* mosquitto

* Twitter
	* tweepy

* 言語
	* python
	* html
	* JavaScript
	* ShellScript

__ESP-WROOM-02__

* MQTT
	* Adafruit_MQTT.h
	* Adafruit_MQTT_Client.h

* 赤外線送信
	* IRremoteESP8266.h

* 言語
	* Arduino



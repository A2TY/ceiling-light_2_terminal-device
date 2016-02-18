# module-IoT

## 概要
センサとアウトプット部分を個々に分け，センサデータに応じた動作をユーザによって自由に設定できるIoT環境の構築．センサデバイスおよびアウトプットデバイスは，ESP-WROOM-02を使用．デバイス同士の通信は，MQTTを使用．

リモコン・レシピ管理サイトはBootstrapテーマ「Honoka」を使用．
[http://honokak.osaka/](http://honokak.osaka/)

## 使用した環境および言語
__Server__
* OS
	* CentOS 6.7

* WebSocket
	* tornado

* MQTT
	* mosquitto

* Twitter
	* tweepy

* 言語
	* python
	* HTML
	* JavaScript
	* ShellScript

__Client__
* メインプラットフォーム
	* ESP-WROOM-02

* センサ
	* 人感センサ　　[http://akizukidenshi.com/catalog/g/gM-09002/](http://akizukidenshi.com/catalog/g/gM-09002/)
	* 温湿度センサ　[http://akizukidenshi.com/catalog/g/gM-08775/](http://akizukidenshi.com/catalog/g/gM-08775/)
	* 

* MQTT
	* Adafruit_MQTT.h
	* Adafruit_MQTT_Client.h

* 赤外線送信
	* IRremoteESP8266.h

* 言語
	* Arduino

#!/bin/sh

#30分毎にTwitterから電車の運行情報と天気予報を取得
up_scraper() {
    while true
    do
        sleep 1800
        python3 /home/pi/dev/project_c2t/scraper.py
    done
}

#MQTTServerとWebSocketServerを起動
mosquitto & python3 /home/pi/dev/project_c2t/server.py & up_scraper

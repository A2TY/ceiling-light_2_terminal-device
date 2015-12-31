/***************************************************
    MQTT通信を用いて，テレビ（東芝REGZA）の赤外線リモコン

    赤外線送信はNECフォーマットを使用

    ライブラリ
        https://github.com/adafruit/Adafruit_MQTT_Library
        https://github.com/markszabo/IRremoteESP8266
 ****************************************************/

#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <IRremoteESP8266.h>

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "___SSID___"
#define WLAN_PASS       "___PASS___"

/***************************** Your Setup ************************************/

#define YOUR_SERVER      "___ADDRESS___"
#define YOUR_SERVERPORT  1883
#define YOUR_USERNAME    "...your username..."
#define YOUR_PASSWORD    "...your password..."

/*************************** Global State ************************************/

// MQTT serverに接続するESP8266 WiFiClientクラスを作成
WiFiClient client;

// MQTT server，クライアントID，ユーザ名，およびパスワードを保管
const char MQTT_SERVER[] PROGMEM    = YOUR_SERVER;
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ "_test_client_id";
const char MQTT_USERNAME[] PROGMEM  = YOUR_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = YOUR_PASSWORD;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, YOUR_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Topic ***************************************/

// pubulisher & subscriberの設定
const char TEST_TOPIC[] PROGMEM = "test";
const char TEST_TOPIC2[] PROGMEM = "test2";
Adafruit_MQTT_Publish testPublisher = Adafruit_MQTT_Publish(&mqtt, TEST_TOPIC);
Adafruit_MQTT_Subscribe testSubscriber = Adafruit_MQTT_Subscribe(&mqtt, TEST_TOPIC);
Adafruit_MQTT_Subscribe test2Subscriber = Adafruit_MQTT_Subscribe(&mqtt, TEST_TOPIC2);

/*************************** Sketch Code ************************************/

IRsend irsend(5); // GPIO 5を赤外線送信に使用

//#define IR_LED 5

void setup() {
    Serial.begin(115200);
    irsend.begin();
    delay(10);

    Serial.println(F("MQTT demo"));

    // Wi-Fiに接続
    Serial.println(); Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WLAN_SSID);

    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.println("WiFi connected");
    Serial.println("IP address: "); Serial.println(WiFi.localIP());

    // MQTT subscriptionを設定
    mqtt.subscribe(&testSubscriber);
    mqtt.subscribe(&test2Subscriber);
}

// TVに赤外線送信
void sendIrData(String data) {
    if (data == "power") {
        irsend.sendNEC(0x2FD48B7, 32);
    }
    if (data == "1ch") {
        irsend.sendNEC(0x2FD807F, 32);
    }
    if (data == "2ch") {
        irsend.sendNEC(0x2FD40BF, 32);
    }
    if (data == "3ch") {
        irsend.sendNEC(0x2FDC03F, 32);
    }
    if (data == "4ch") {
        irsend.sendNEC(0x2FD20DF, 32);
    }
    if (data == "5ch") {
        irsend.sendNEC(0x2FDA05F, 32);
    }
    if (data == "6ch") {
        irsend.sendNEC(0x2FD609F, 32);
    }
    if (data == "7ch") {
        irsend.sendNEC(0x2FDE01F, 32);
    }
    if (data == "8ch") {
        irsend.sendNEC(0x2FD10EF, 32);
    }
    if (data == "9ch") {
        irsend.sendNEC(0x2FD906F, 32);
    }
    if (data == "10ch") {
        irsend.sendNEC(0x2FD50AF, 32);
    }
    if (data == "11ch") {
        irsend.sendNEC(0x2FDD02F, 32);
    }
    if (data == "12ch") {
        irsend.sendNEC(0x2FD30CF, 32);
    }
    if (data == "ch_up") {
        irsend.sendNEC(0x2FDD827, 32);
    }
    if (data == "ch_down") {
        irsend.sendNEC(0x2FDF807, 32);
    }
    if (data == "vol_up") {
        irsend.sendNEC(0x2FD58A7, 32);
    }
    if (data == "vol_down") {
        irsend.sendNEC(0x2FD7887, 32);
    }
}


uint32_t x=0;

void loop() {
    MQTT_connect();
    String recevIrData;
    //char remocon;

    // topicを監視しsubscriptionのmessageを受け取る
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(1000))) {

        if (subscription == &testSubscriber) {
            Serial.print(F("Got: "));
            Serial.println((char *)testSubscriber.lastread);
            //led = atoi((char *)testSubscriber.lastread); // char型をint型に変換
            recevIrData = String((char *)testSubscriber.lastread); // subscriptionのmessageを受け取る
            sendIrData(recevIrData);

    //     remocon = *testSubscriber.lastread;
    //     Serial.println(remocon);
    //     if (led == 1) {
    //      int dataSize = sizeof(tv_volup) / sizeof(tv_volup[0]);
    //      Serial.println(dataSize);
    //      for (int cnt = 0; cnt < dataSize; cnt++) {
    //        unsigned long len = tv_volup[cnt]*10;  // dataは10us単位でON/OFF時間を記録している
    //        unsigned long us = micros();
    //        do {
    //          digitalWrite(IR_LED, 1 - (cnt&1)); // cntが偶数なら赤外線ON、奇数ならOFFのまま
    //          delayMicroseconds(8);  // キャリア周波数38kHzでON/OFFするよう時間調整
    //          digitalWrite(IR_LED, 0);
    //          delayMicroseconds(7);
    //          Serial.print(len + ", ");
    //        } while (long(us + len - micros()) > 0); // 送信時間に達するまでループ
    //      }
    //     }
    //     led = atoi((char *)testSubscriber.lastread); //char型をint型に変換
    //     Serial.println(led);
    //     //指定したピン番号のLEDを点灯
    //     if (led == 12) {
    //      digitalWrite(12, HIGH);
    //     } else if (led == 13) {
    //      digitalWrite(13, HIGH);
    //     } else if (led == 14) {
    //      digitalWrite(14, HIGH);
    //     } else {
    //      digitalWrite(12, LOW);
    //      digitalWrite(13, LOW);
    //      digitalWrite(14, LOW);
    //     }

        }
        if (subscription == &test2Subscriber) {
            Serial.print(F("Got: "));
            Serial.println((char *)test2Subscriber.lastread);
        }
    }

      //testPublisher.publish(1);

        delay(1000);
}

// MQTT serverの接続状況を調べ切断されていれば再接続
void MQTT_connect() {
    int8_t ret;

    // MQTT serverの接続状況を調べる
    if (mqtt.connected()) {
        return;
    }

    Serial.print("Connecting to MQTT... ");

    while ((ret = mqtt.connect()) != 0) { // 接続された場合は0を返す　接続されない場合は5秒おきに接続を繰り返す
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println(ret);
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000);  // 5秒間待機
    }
    Serial.println("MQTT Connected!");
}
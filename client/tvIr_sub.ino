/*****************************************************************************
  MQTT topic "tvIr"をSubした時にTVに赤外線送信を行う

  赤外線送信はNECフォーマットを使用

  https://github.com/adafruit/Adafruit_MQTT_Library
  https://github.com/markszabo/IRremoteESP8266
 *****************************************************************************/

#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <IRremoteESP8266.h>
#include <String.h>

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "___SSID___"
#define WLAN_PASS       "___PASS___"

/***************************** Your Setup ************************************/

#define YOUR_SERVER      "___ADDRESS___"
#define YOUR_SERVERPORT  ___PORTS___
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
const char TOPIC_TVIR[] PROGMEM = "/server/tvIr";

Adafruit_MQTT_Subscribe tvIrSubscriber = Adafruit_MQTT_Subscribe(&mqtt, TOPIC_TVIR);

/*************************** Sketch Code ************************************/


IRsend irsend(5); // GPIO 5を赤外線送信に使用

void setup() {
  Serial.begin(115200);
  irsend.begin();

  // Wi-Fiに接続
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  //WiFi.config(IPAddress(), WiFi.gatewayIP(), WiFi.subnetMask());
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // MQTT subscriptionを設定
  mqtt.subscribe(&tvIrSubscriber);
}

// TVに赤外線送信
void sendIrData(String data) {
  for (int i = 0; i <= data.length(); i++) {
    String dataS = data.substring(i, i+1);
    char dataC[32];
    dataS.toCharArray(dataC, 32);  //char型に変換
    switch (dataC[0]) {
      case 'p':
        Serial.println("p");
        irsend.sendNEC(0x2FD48B7, 32);
        break;
      case '1':
        Serial.println("1");
        irsend.sendNEC(0x2FD807F, 32);
        break;
      case '2':
        Serial.println("2");
        irsend.sendNEC(0x2FD40BF, 32);
        break;
      case '3':
        Serial.println("3");
        irsend.sendNEC(0x2FDC03F, 32);
        break;
      case '4':
        Serial.println("4");
        irsend.sendNEC(0x2FD20DF, 32);
        break;
      case '5':
        Serial.println("5");
        irsend.sendNEC(0x2FDA05F, 32);
        break;
      case '6':
        Serial.println("6");
        irsend.sendNEC(0x2FD609F, 32);
        break;
      case '7':
        Serial.println("7");
        irsend.sendNEC(0x2FDE01F, 32);
        break;
      case '8':
        Serial.println("8");
        irsend.sendNEC(0x2FD10EF, 32);
        break;
      case '9':
        Serial.println("9");
        irsend.sendNEC(0x2FD906F, 32);
        break;
      case 'x':
        Serial.println("x");
        irsend.sendNEC(0x2FD50AF, 32);
        break;
      case 'y':
        Serial.println("y");
        irsend.sendNEC(0x2FDD02F, 32);
        break;
      case 'z':
        Serial.println("z");
        irsend.sendNEC(0x2FD30CF, 32);
        break;
      case 'C':
        Serial.println("C");
        irsend.sendNEC(0x2FDD827, 32);
        break;
      case 'c':
        Serial.println("c");
        irsend.sendNEC(0x2FDF807, 32);
        break;
      case 'V':
        Serial.println("V");
        irsend.sendNEC(0x2FD58A7, 32);
        break;
      case 'v':
        Serial.println("v");
        irsend.sendNEC(0x2FD7887, 32);
        break;
    }
    if (data.length() != 1) {
      delay(2000);
    }
  }
}

void loop() {
  MQTT_connect();
  
  String recevIrData;

  // topicを監視しsubscriptionのmessageを受け取る
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &tvIrSubscriber) {
      Serial.print(F("Got: "));
      Serial.println((char *)tvIrSubscriber.lastread);
      recevIrData = String((char *)tvIrSubscriber.lastread);  // subscriptionのmessageを受け取る
      sendIrData(recevIrData);
    }
  }
  
  // MQTTブローカにpingを送る
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }

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



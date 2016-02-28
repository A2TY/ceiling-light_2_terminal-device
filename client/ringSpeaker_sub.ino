/*****************************************************************************
  MQTT topic "ringSpeaker"をSubした時にブザーを鳴らす

  https://github.com/adafruit/Adafruit_MQTT_Library
 *****************************************************************************/

#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

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

/****************************** Topic ***************************************//////////////////////////////////////

// pubulisher & subscriberの設定
const char TOPIC_RINGSPEAKER[] PROGMEM = "/server/ringSpeaker";

Adafruit_MQTT_Subscribe ringSpeakerSubscriber = Adafruit_MQTT_Subscribe(&mqtt, TOPIC_RINGSPEAKER);

/*************************** Sketch Code ************************************//////////////////////////////////////


static const int speakerPin = 16;

void setup() {
  Serial.begin(115200);
  pinMode(speakerPin, OUTPUT);

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
  mqtt.subscribe(&ringSpeakerSubscriber);
}

void loop() {
  MQTT_connect();

  //topicをPubし，"ringSpeaker"をPubした時にブザーを鳴らす
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &ringSpeakerSubscriber) {
      for (int i = 0; i < 30; i++) {
        analogWrite(speakerPin, 500);
        delay(50);
        analogWrite(speakerPin, 0);
        delay(50);
      }
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



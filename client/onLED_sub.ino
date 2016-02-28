/*****************************************************************************
  MQTT topic "onLED"をSubした時にLEDを点灯させる

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

/****************************** Topic ***************************************/

// pubulisher & subscriberの設定
const char TOPIC_ONLED_COLOR[] PROGMEM = "/server/onLED/color";
const char TOPIC_ONLED_TIME[] PROGMEM = "/server/onLED/time";
const char TOPIC_ONLED_INTERVAL[] PROGMEM = "/server/onLED/interval";

Adafruit_MQTT_Subscribe onLEDColorSubscriber = Adafruit_MQTT_Subscribe(&mqtt, TOPIC_ONLED_COLOR);
Adafruit_MQTT_Subscribe onLEDTimeSubscriber = Adafruit_MQTT_Subscribe(&mqtt, TOPIC_ONLED_TIME);
Adafruit_MQTT_Subscribe onLEDIntervalSubscriber = Adafruit_MQTT_Subscribe(&mqtt, TOPIC_ONLED_INTERVAL);

/*************************** Sketch Code ************************************/


void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(16, OUTPUT);

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
  mqtt.subscribe(&onLEDColorSubscriber);
  mqtt.subscribe(&onLEDTimeSubscriber);
  mqtt.subscribe(&onLEDIntervalSubscriber);
}

int timeDataInt = 0;
int intervalDataInt = 0;

void setupLED(String colorData, int timeData, int intervalData) {
  Serial.println("setupLED");
  Serial.println(timeData/(2*intervalData));
  timeDataInt = 0;
  intervalDataInt = 0;
  if (colorData.indexOf("赤") != -1 || colorData.indexOf('r') != -1) {
    Serial.println("赤");
    for (int i = 0; i < timeData/(2*intervalData); i++) {
      digitalWrite(4, HIGH);
      delay(intervalData);
      digitalWrite(4, LOW);
      delay(intervalData);
    }
  } else if (colorData.indexOf("緑") != -1 || colorData.indexOf('g') != -1) {
    Serial.println("緑");
    for (int i = 0; i < timeData/(2*intervalData); i++) {
      digitalWrite(5, HIGH);
      delay(intervalData);
      digitalWrite(5, LOW);
      delay(intervalData);
    }
  } else if (colorData.indexOf("青") != -1 || colorData.indexOf('b') != -1) {
    Serial.println("青");
    for (int i = 0; i < timeData/(2*intervalData); i++) {
      digitalWrite(16, HIGH);
      delay(intervalData);
      digitalWrite(16, LOW);
      delay(intervalData);
    }
  }
}

void loop() {
  MQTT_connect();
  String colorData;
  String timeData;
  String intervalData;

  //topicをPubし，"onLED"をPubした時にLEDを点灯させる
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &onLEDColorSubscriber) {
      Serial.print(F("Got: "));
      Serial.println((char *)onLEDColorSubscriber.lastread);
      colorData = String((char *)onLEDColorSubscriber.lastread);
    }
    if (subscription == &onLEDTimeSubscriber) {
      Serial.print(F("Got: "));
      Serial.println((char *)onLEDTimeSubscriber.lastread);
      timeData = String((char *)onLEDTimeSubscriber.lastread);
    }
    if (subscription == &onLEDIntervalSubscriber) {
      Serial.print(F("Got: "));
      Serial.println((char *)onLEDIntervalSubscriber.lastread);
      intervalData = String((char *)onLEDIntervalSubscriber.lastread);
    }
    char timeDataChar[32];
    timeData.toCharArray(timeDataChar, 32);
    timeDataInt = atoi(timeDataChar);
    Serial.println(timeDataInt);
    char intervalDataChar[32];
    intervalData.toCharArray(intervalDataChar, 32);
    intervalDataInt = atoi(intervalDataChar);
    Serial.println(intervalDataInt);
    if (timeDataInt != 0 && intervalDataInt != 0) {
      setupLED(colorData, timeDataInt, intervalDataInt);
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



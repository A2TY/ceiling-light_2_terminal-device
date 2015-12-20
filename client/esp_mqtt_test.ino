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

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Store the MQTT server, client ID, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = YOUR_SERVER;
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ "_test_client_id";
const char MQTT_USERNAME[] PROGMEM  = YOUR_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = YOUR_PASSWORD;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, YOUR_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Topic ***************************************/

// Setup a pubulisher & subscriber.
const char TEST_TOPIC[] PROGMEM = "test";
const char TEST_TOPIC2[] PROGMEM = "test2";
Adafruit_MQTT_Publish testPublisher = Adafruit_MQTT_Publish(&mqtt, TEST_TOPIC);
Adafruit_MQTT_Subscribe testSubscriber = Adafruit_MQTT_Subscribe(&mqtt, TEST_TOPIC);
Adafruit_MQTT_Subscribe test2Subscriber = Adafruit_MQTT_Subscribe(&mqtt, TEST_TOPIC2);

/*************************** Sketch Code ************************************/

void setup() {
    Serial.begin(115200);
    delay(10);

    Serial.println(F("MQTT demo"));

    // Connect to WiFi access point.
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

    // Setup MQTT subscription.
    mqtt.subscribe(&testSubscriber);
    mqtt.subscribe(&test2Subscriber);
}

uint32_t x=0;
IRsend irsend(5); //an IR led is connected to GPIO pin 0

void loop() {
    // Ensure the connection to the MQTT server is alive (this will make the first
    // connection and automatically reconnect when disconnected).  See the MQTT_connect
    // function definition further below.
    MQTT_connect();
    int led;

    // this is our 'wait for incoming subscription packets' busy subloop
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &testSubscriber) {
        Serial.print(F("Got: "));
        Serial.println((char *)testSubscriber.lastread);
        led = atoi((char *)testSubscriber.lastread); //char型をint型に変換
        switch (led) {
            case 0 :
                irsend.sendNEC(0x2FD48B7, 32);
                break;
            case 1 :
                irsend.sendNEC(0x2FD807F, 32);
                break;
            case 2 :
                irsend.sendNEC(0x2FD40BF, 32);
                break;
            case 3 :
                irsend.sendNEC(0x2FDC03F, 32);
                break;
            case 4 :
                irsend.sendNEC(0x2FD20DF, 32);
                break;
            case 5 :
                irsend.sendNEC(0x2FDA05F, 32);
                break;
            case 6 :
                irsend.sendNEC(0x2FD609F, 32);
                break;
            case 7 :
                irsend.sendNEC(0x2FDE01F, 32);
                break;
            case 8 :
                irsend.sendNEC(0x2FD10EF, 32);
                break;
            case 9 :
                irsend.sendNEC(0x2FD906F, 32);
                break;
            case 10 :
                irsend.sendNEC(0x2FD50AF, 32);
                break;
            case 11 :
                irsend.sendNEC(0x2FDD02F, 32);
                break;
            case 12 :
                irsend.sendNEC(0x2FD30CF, 32);
                break;
            case 13 :
                irsend.sendNEC(0x2FDD827, 32);
                break;
            case 14 :
                irsend.sendNEC(0x2FDF807, 32);
                break;
            case 15 :
                irsend.sendNEC(0x2FD58A7, 32);
                break;
            case 16 :
                irsend.sendNEC(0x2FD7887, 32);
                break;
        }
    }
    if (subscription == &test2Subscriber) {
        Serial.print(F("Got: "));
        Serial.println((char *)test2Subscriber.lastread);
    }
}

    //testPublisher.publish(1);

    delay(1000);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
    int8_t ret;

    // Stop if already connected.
    if (mqtt.connected()) {
        return;
    }

    Serial.print("Connecting to MQTT... ");

    while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println(ret);
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000);  // wait 5 seconds
    }
    Serial.println("MQTT Connected!");
}

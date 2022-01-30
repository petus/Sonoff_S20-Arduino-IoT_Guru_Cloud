/* written by chiptron.cz */
/* 2022 */

#include <IoTGuru.h>
#include <ESP8266WiFi.h>

#define RELAY_PIN         12
#define LED_PIN           13
#define BUTTON_PIN        0

/**
 * WiFi parameters.
 */
const char* ssid        = "ssid";
const char* password    = "pass";
WiFiClient client;

/**
 * Initialize the connection with the cloud.
 */
String userShortId      = "userShortID";
String deviceShortId    = "deviceShortID";
String deviceKey        = "deviceKey";
IoTGuru iotGuru         = IoTGuru(userShortId, deviceShortId, deviceKey);

/**
 * Constants of the MQTT channel check.
 */
String nodeShortId      = "NodeShortID";
String fieldName        = "general_plug"; // change based on your name

void setup() {
    Serial.begin(115200);
    delay(10);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);

    WiFi.begin(ssid, password);
    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print(".");
        digitalWrite(LED_PIN, LOW);
        count++;
        if(count > 30)
        {
          digitalWrite(LED_PIN, LOW);
          delay(500);
          digitalWrite(LED_PIN, HIGH);
          delay(500);
          if(count > 40)
          {
            count = 0;
          }
        }
    }
    Serial.println("");
    digitalWrite(LED_PIN, HIGH);

    /**
     * Set the relay pin.
     */
    pinMode(RELAY_PIN, OUTPUT);

    /**
     * Set the callback function.
     */
    iotGuru.setCallback(&callback);
    /**
     * Set the debug printer (optional).
     */
    iotGuru.setDebugPrinter(&Serial);
    /**
     * Set the network client.
     */
    iotGuru.setNetworkClient(&client);
}

void loop() {
    iotGuru.loop();
    
    // if button is press, turn off the relay
    if(digitalRead(BUTTON_PIN) == LOW)
    {
      Serial.println("Manually switch relay to LOW");
      digitalWrite(RELAY_PIN, LOW);
      delay(500);
    }
}

// callback
void callback(const char* cbNodeShortId, const char* cbFieldName, const char* message) {
    Serial.print(cbNodeShortId);Serial.print(" - ");Serial.print(cbFieldName);Serial.print(": ");Serial.println(message);

    if (strcmp(cbNodeShortId, nodeShortId.c_str()) == 0) {
        if (strcmp(cbFieldName, fieldName.c_str()) == 0) {
            if (strcmp(message, "0") == 0) {
                Serial.println("Switch relay to LOW");
                digitalWrite(RELAY_PIN, LOW);
            } else {
                Serial.println("Switch relay to HIGH");
                digitalWrite(RELAY_PIN, HIGH);
            }
        }
    }
}

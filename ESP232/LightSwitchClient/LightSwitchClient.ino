#include "Button.h"
#include "LED.h"
#include <WiFi.h>
#include "PubSubClient.h"
#include <string.h>

Button buttons[] = { (23), (22), (21), (15) };
LED LEDs[] = { (32), (33), (25), (26) };
#define N_SIZE 4

// WiFi
const char *ssid = "l0rdturn1p";      // WiFi name
const char *password = "travshouse";  // WiFi password

// MQTT Broker
const char *mqtt_broker = "10.0.0.105";  // Broker IP
const std::string mqtt_topic = "Apartment-Travis";
const int mqtt_port = 1883;
const char *mqtt_username = "";  // not required
const char *mqtt_password = "";  // not required

void callback(char *topic, byte *payload, unsigned int length);

// Client
WiFiClient espClient;
PubSubClient client(mqtt_broker, mqtt_port, callback, espClient);
const char *mqtt_client_name = "ESP32Client";

void callback(char *topic, byte *payload, unsigned int length) {
  //Convert byte payload into a char array
  char message[length+1];
  memcpy(message,payload,length);
  message[length] =0;
  char result = message[length-1];

  //Get the light number from the topic
  size_t len = strlen(topic);
  int light = topic[len-1] - '0';

  if(result == '1'){
    LEDs[light-1].setState(HIGH);
  }
  else {
    LEDs[light-1].setState(LOW);
  }
}

void setupWiFi() {
  WiFi.begin(ssid, password);
  unsigned long startTime = millis();

  // Continually check status until connected or timeout
  while (WiFi.status() != WL_CONNECTED) {

    // timeout
    if (millis() - startTime > 20000) {
      Serial.println("Faild to connect. Connection timed out.");
      return;
    }

    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi.");
}

void setupMQTT() {
  unsigned long startTime = millis();

  // Continually check status until connected or timeout
  while (!client.connected()) {
    // timeout

    if (millis() - startTime > 10000) {
      Serial.println("Client couldn't connect. Connection timed out.");
      return;
    }

    Serial.println("Connecting to MQTT broker...");

    if (client.connect(mqtt_client_name)) {
      Serial.println("Connected to MQTT broker.");
    } else {

      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying...");
      delay(500);
    }
  }

  client.subscribe(mqtt_topic.c_str());
  client.publish(mqtt_topic.c_str(), "Initialized");
}

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupMQTT();
    //Setup buttons and LEDs
  
  for (unsigned n = 0; n < N_SIZE; n++) {
    buttons[n].begin();
    LEDs[n].begin();

    //Create a topic for each light
    std::string topic = std::string(mqtt_topic + "/" + std::to_string(n + 1));
    client.subscribe(topic.c_str());
    client.publish(topic.c_str(), "Initialized");
  }

  Serial.println("Setup done");
}

// the loop function runs over and over again forever
void loop() {
  client.loop();

  for (unsigned n = 0; n < N_SIZE; n++) {
    if (buttons[n].released()) {
      bool result = !LEDs[n].getState();
      
      std::string message = std::string(std::to_string(result));
      std::string topic = std::string(mqtt_topic + "/" + std::to_string(n + 1) );

      client.publish(topic.c_str(), message.c_str(),true);
    }
  }
}

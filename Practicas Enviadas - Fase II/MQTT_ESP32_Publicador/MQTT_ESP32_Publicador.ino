// CLASE IoT 01/10/2024: MQQT (EMQX)

#include "WiFi.h"
#include "PubSubClient.h"

// Desired resolution for measurements
#define ADC_VREF_mV 3300.0
#define ADC_RESOLUTION 4096.0
WiFiClient esp32Client;
PubSubClient mqttClient(esp32Client);

// WiFi credentials
const char* ssid = "jRedmi 10C";
const char* password = "123456789.";

// Broker details
char* server = "broker.emqx.io";
int port = 1883;

// Pin definitions
int temp = 33;
int led = 2;

// Variables
char datos[40];

void wifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  digitalWrite(led, HIGH);
  Serial.println("");
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("ESP32Publisher")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  delay(10);
  wifi();
  mqttClient.setServer(server, port);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
 
  float temperatura = (analogRead(temp) * (ADC_VREF_mV / ADC_RESOLUTION)) / 10.0 + 8;
  Serial.print("Temperature: ");
  Serial.println(temperatura);
  sprintf(datos, "%.2f", temperatura);
  mqttClient.publish("test_IoT/", datos);
  delay(5000);


}

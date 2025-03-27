#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "sensorData.h"

// WiFi credentials
const char* ssid = "PISO_2";
const char* password = "Planetaland2";

// Broker details
const char* server = "broker.emqx.io";
const int port = 1883;

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Pin definitions
const int ledWiFi = LED_BUILTIN; // LED interno de la ESP8266
const int ledMQTTSuccess = D3;   // LED externo para conexión exitosa a MQTT
const int ledMQTTFail = D4;      // LED externo para fallo de conexión a MQTT

// Variables
char datosTemperatura[40];

void wifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Intentar conexión a WiFi
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledWiFi, LOW);  // Parpadeo del LED interno mientras conecta
    delay(500);
    digitalWrite(ledWiFi, HIGH);
    delay(500);
  }
  digitalWrite(ledWiFi, LOW);    // Encender LED interno cuando está conectado
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("ESP8266Publisher")) {
      Serial.println("Connected");
      digitalWrite(ledMQTTSuccess, HIGH); // LED de conexión exitosa
      digitalWrite(ledMQTTFail, LOW);     // Apaga LED de fallo
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Retrying in 5 seconds");
      digitalWrite(ledMQTTSuccess, LOW);  // Apaga LED de éxito
      digitalWrite(ledMQTTFail, HIGH);    // Enciende LED de fallo
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ledWiFi, OUTPUT);
  pinMode(ledMQTTSuccess, OUTPUT);
  pinMode(ledMQTTFail, OUTPUT);
  digitalWrite(ledWiFi, HIGH);        // Apagar LED interno inicialmente
  digitalWrite(ledMQTTSuccess, LOW);   // Apagar LED de éxito inicialmente
  digitalWrite(ledMQTTFail, LOW);      // Apagar LED de fallo inicialmente
  Serial.begin(115200);
  wifi();
  mqttClient.setServer(server, port);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  DatosSensor sensorData = generarDatosSensor(1); // Ajusta según tu ciudad

  Serial.print("Temperature: ");
  Serial.println(sensorData.temperatura);

  sprintf(datosTemperatura, "%.2f", sensorData.temperatura);
  mqttClient.publish("Paris", datosTemperatura); // Ajusta con tu ciudad
}

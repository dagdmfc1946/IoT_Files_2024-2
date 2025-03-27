#include <WiFi.h>
#include <PubSubClient.h>
#include "sensorData.h"

// Desired resolution for measurements
//#define ADC_VREF_mV 3300.0
//#define ADC_RESOLUTION 4096.0
WiFiClient esp32Client;
PubSubClient mqttClient(esp32Client);

// WiFi credentials
//const char* ssid = "PISO_2";
//const char* password = "Planetaland2";

const char* ssid = "DAGDMFC";
const char* password = "millos2017";

// Broker details
char* server = "broker.emqx.io";
int port = 1883;

// Pin definitions
int temp = 4;             // Pin para el sensor de temperatura
int wifiLed = 2;          // Pin para LED de conexión Wi-Fi
int mqttLedGreen = 13;    // LED verde para indicar conexión a MQTT
int mqttLedRed = 12;      // LED rojo para indicar desconexión de MQTT

// Variables
char datosTemperatura[40];
char datosHumedad[40];

void wifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  digitalWrite(wifiLed, HIGH);  // Enciende el LED de Wi-Fi
  Serial.println("");
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Conexión MQTT perdida, intentando conectarse nuevamente...");
    if (mqttClient.connect("ESP32Publisher")) {
      Serial.println(" Conectado");
      digitalWrite(mqttLedGreen, HIGH); // Enciende LED verde para MQTT
      digitalWrite(mqttLedRed, LOW);    // Apaga LED rojo para MQTT
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Intentar de nuevo en 5 segundos");
      digitalWrite(mqttLedGreen, LOW);  // Apaga LED verde
      digitalWrite(mqttLedRed, HIGH);   // Enciende LED rojo
      delay(3000); // Espera de 3.5 segundos
    }
  }
}

void setup() {
  pinMode(temp, INPUT);          // Configura el pin de temperatura como entrada
  pinMode(wifiLed, OUTPUT);
  pinMode(mqttLedGreen, OUTPUT);
  pinMode(mqttLedRed, OUTPUT);
  Serial.begin(115200);
  randomSeed(analogRead(0));  // Semilla para el generador de números aleatorio
  delay(10);
  wifi();
  mqttClient.setServer(server, port);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  DatosSensor sensorData = generarDatosSensor(5); // Ingresa el número asociado a la ciudad del grupo

  Serial.print("Temperature: ");
  Serial.println(sensorData.temperatura);

  // Formatea el valor de temperatura en el buffer de datos
  sprintf(datosTemperatura, "%.3f", sensorData.temperatura);

  // Publica el dato de temperatura en el tópico MQTT
  mqttClient.publish("Cancun", datosTemperatura); // Escribe la ciudad asociada al grupo
}

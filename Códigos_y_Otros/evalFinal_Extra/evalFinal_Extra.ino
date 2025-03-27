// Librerías a usar
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// Configuración del DHT11
#define DHTPIN 4        // Pin para el sensor DHT11
#define DHTTYPE DHT11   // Tipo de sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Datos de la WiFi
WiFiClient esp32Client;
const char* ssid = "DAGDMFC";
const char* password = "millos2017";

// Datos Broker y clientes
PubSubClient mqttClient(esp32Client);
const char* server = "172.171.224.96"; // La IP pública de la VM en Azure
int port = 1883; // el puerto que tiene Mosquitto asignado, por defecto 1883
char datos[50]; // Variable usada para los valores de las mediciones

// Datos del sensor y los pines de la ESP32
int pinLedDistancia = 27; // LED que enciende cuando la distancia es menor a 20 cm
int echoPin = 33;         // Pin de Echo del sensor HC-SR04
int triggerPin = 32;      // Pin de Trigger del sensor HC-SR04
int ledWiFi = 2;          // LED indicador de conexión WiFi
int ledMQTTConectado = 13; // LED para indicar conexión MQTT exitosa
int ledMQTTDesconectado = 12; // LED para indicar falla en conexión MQTT

// Función para calcular la distancia en cm usando el sensor ultrasónico
long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

// Conexión a la WiFi
void wifi() {
  Serial.print("Conectándose a WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { // Mostrar puntitos mientras no logra conexión
    Serial.print(".");
    delay(500);
  }
  digitalWrite(ledWiFi, HIGH); // Encender el LED al conectar WiFi
  Serial.println("\nConectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

// Reconexión al broker MQTT
void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Conexión MQTT perdida, intentando reconectar...");
    if (mqttClient.connect("esp32")) {
      Serial.println("Conectado al broker MQTT");
      digitalWrite(ledMQTTConectado, HIGH); // Encender LED MQTT cuando esté conectado
      digitalWrite(ledMQTTDesconectado, LOW); // Apagar LED de desconexión
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" intentar de nuevo en 5 segundos");
      digitalWrite(ledMQTTConectado, LOW); // Apagar LED MQTT si la conexión falla
      digitalWrite(ledMQTTDesconectado, HIGH); // Encender LED de desconexión
      delay(5000); // Espera de 5 segundos
    }
  }
}

void setup() {
  // Configuración de pines
  pinMode(ledWiFi, OUTPUT);         // LED indicador de WiFi
  pinMode(pinLedDistancia, OUTPUT); // LED indicador de distancia
  pinMode(ledMQTTConectado, OUTPUT); // LED indicador de conexión MQTT
  pinMode(ledMQTTDesconectado, OUTPUT); // LED indicador de desconexión MQTT

  // Configuración del monitor serial y del sensor DHT11
  Serial.begin(115200);
  delay(10);
  dht.begin();

  // Conexión a WiFi y configuración del servidor MQTT
  wifi();
  mqttClient.setServer(server, port);
}

void loop() {
  // Si se pierde la conexión WiFi, intentar reconectar
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Conexión WiFi perdida. Intentando reconectar...");
    digitalWrite(ledWiFi, LOW); // Apagar LED WiFi
    wifi(); // Intentar reconectar
  }

  // Si se pierde la conexión con el broker MQTT, intentar reconectar
  if (!mqttClient.connected()) {
    reconnect();
  }

  // Calcular la distancia medida por el sensor ultrasónico
  int distancia = 0.01723 * readUltrasonicDistance(triggerPin, echoPin);

  // Encender el LED si la distancia es menor a 20 cm
  if (distancia < 20) {
    digitalWrite(pinLedDistancia, HIGH);
  } else {
    digitalWrite(pinLedDistancia, LOW);
  }

  // Leer la humedad y temperatura del sensor DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Verificar si las lecturas del sensor DHT11 son válidas
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Fallo al leer el sensor DHT!"));
  } else {
    // Publicar la distancia, temperatura y humedad en el broker MQTT
    sprintf(datos, "Distancia: %d cm, Temp: %.1f°C, Humedad: %.1f%%", distancia, t, h);
    mqttClient.publish("evaluacionFinal", datos);

    // Mostrar en el monitor serial
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.print("%  Temperatura: ");
    Serial.print(t);
    Serial.println("°C  Distancia: ");
    Serial.print(distancia);
    Serial.print("cm");
  }

  delay(3000); // Espera antes de la siguiente lectura
}


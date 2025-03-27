// Librerias a usar
#include "WiFi.h"
#include "PubSubClient.h"

// Resolucion deseada en las mediciones
#define ADC_VREF_mV 3300.0
#define ADC_RESOLUTION 4096.0

// Datos de la WiFi
WiFiClient esp32Client;
const char* ssid = "PISO_2";
const char* password = "Planetaland2";

// Datos Broker y clientes
PubSubClient mqttClient(esp32Client);
const char* server = "172.171.224.96"; // La IP pública de la VM en AWS
int port = 1883; // el puerto que tiene Mosquitto asignado, por defecto 1883
char datos[40]; // variable usada para los valores de las mediciones

// Datos del sensor y los pines de la ESP32
int pinLed = 4;          // LED que enciende cuando la distancia es menor a 20 cm
int echoPin = 13;         // Pin de Echo del sensor HC-SR04
int triggerPin = 12;     // Pin de Trigger del sensor HC-SR04
int ledWiFi = 2;         // LED indicador de conexión WiFi

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
  while (WiFi.status() != WL_CONNECTED) { // Mostrar punticos mientras no logra conexión
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
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" intentar de nuevo en 5 segundos");
      delay(5000); // Espera de 5 segundos
    }
  }
}

void setup() {
  pinMode(ledWiFi, OUTPUT); // Configuración del LED indicador de WiFi
  pinMode(pinLed, OUTPUT);   // Configuración del LED para la distancia
  Serial.begin(115200);      // Monitor serial
  delay(10);
  
  wifi();                     // Conexión a WiFi
  mqttClient.setServer(server, port); // Conexión al broker MQTT
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
  Serial.print("Distancia: ");
  Serial.println(distancia);

  // Encender el LED si la distancia es menor a 20 cm
  if (distancia < 20) {
    digitalWrite(pinLed, HIGH);
  } else {
    digitalWrite(pinLed, LOW);
  }

  // Enviar la distancia al broker MQTT
  sprintf(datos, "%d", distancia);
  mqttClient.publish("canal_dagd", datos);
  delay(1000); // Espera antes de la siguiente lectura
}

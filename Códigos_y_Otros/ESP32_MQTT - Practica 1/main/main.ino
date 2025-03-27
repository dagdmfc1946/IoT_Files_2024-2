//#include "sensorData.h"

// void setup() {
//   Serial.begin(115200);  // Iniciamos la comunicación serie
//   randomSeed(analogRead(0));  // Semilla para el generador de números aleatorios
// }

// void loop() {
//   // Llamamos a la función que genera los datos, por ejemplo, pasando el valor 3
//   DatosSensor datos = generarDatosSensor(3);

//   // Mostramos los datos en el monitor serial
//   Serial.print("Temperatura: ");
//   Serial.print(datos.temperatura);
//   Serial.print(" °C, Humedad: ");
//   Serial.print(datos.humedad);
//   Serial.println(" %");

//   // No necesitamos delay aquí, ya que la función lo incluye
// }

// CLASE IoT 01/10/2024: MQQT (EMQX)

#include "WiFi.h"
#include "PubSubClient.h"
#include "sensorData.h"  // Incluimos el archivo para generar los datos del sensor

// WiFi y MQTT
WiFiClient esp32Client;
PubSubClient mqttClient(esp32Client);

// Credenciales de WiFi
const char* ssid = "PISO_2";
const char* password = "Planetaland2";

// Broker EMQX
char* server = "broker.emqx.io";
int port = 1883;

// Pines y variables
int led = 2;
char datos[50];  // Ajustamos el tamaño para incluir ambos datos

void wifi() {
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  digitalWrite(led, HIGH);
  Serial.println("");
  Serial.println("Conectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Intentando conectar a MQTT...");
    if (mqttClient.connect("ESP32Publisher")) {
      Serial.println("Conectado");
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Reintentando en 5 segundos");
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
  
  // Generamos los datos del sensor
  DatosSensor datosSensor = generarDatosSensor(3);  // Puedes cambiar el parámetro según lo que desees simular
  
  // Publicamos el valor de la temperatura
  sprintf(datos, "Temperatura: %.2f °C", datosSensor.temperatura);
  mqttClient.publish("test_IoT_temperatura/", datos);
  Serial.println(datos);  // Mostramos la temperatura en el monitor serie

  // Publicamos el valor de la humedad
  sprintf(datos, "Humedad: %.2f %%", datosSensor.humedad);
  mqttClient.publish("test_IoT_humedad/", datos);
  Serial.println(datos);  // Mostramos la humedad en el monitor serie

  // Esperamos 5 segundos antes de la siguiente publicación
  delay(5000);
}

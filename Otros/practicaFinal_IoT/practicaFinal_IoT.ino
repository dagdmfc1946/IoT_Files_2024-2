#include <WiFi.h>
#include <PubSubClient.h>

// Configuración WiFi
const char* ssid = "PISO_2";            // Reemplaza con el nombre de tu red WiFi
const char* password = "Planetaland2";    // Reemplaza con la contraseña de tu red WiFi

// Configuración del broker MQTT
const char* mqtt_broker = "broker.emqx.io";  // Cambia si usas otro broker
const int mqtt_port = 1883;
const char* mqtt_topic = "final_IOT";          // Tópico donde se publicarán los datos

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  // Conexión a WiFi
  Serial.print("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado.");

  // Configuración del cliente MQTT
  client.setServer(mqtt_broker, mqtt_port);
  conectarBrokerMQTT();
}

void loop() {
  if (!client.connected()) {
    conectarBrokerMQTT(); // Reconectar si se pierde la conexión al broker
  }

  // Simular y publicar datos del sensor de temperatura
  publicarDatos();

  // Esperar 5 minutos
  delay(20000);
}

void conectarBrokerMQTT() {
  Serial.print("Conectando al broker MQTT...");
  while (!client.connected()) {
    if (client.connect("ESP32_Client")) { // Nombre único para la ESP32
      Serial.println("\nConectado al broker MQTT.");
    } else {
      Serial.print(".");
      delay(1000);
    }
  }
}

void publicarDatos() {
  // Generar datos simulados del sensor
  String sensor = "ESP32_Temp";
  float temperatura = random(200, 350) / 10.0; // Valores entre 20.0 y 35.0
  String unidades = "°C";

  // Formatear los datos en formato JSON
  String payload = "{";
  payload += "\"sensor\":\"" + sensor + "\",";
  payload += "\"temperatura\":" + String(temperatura) + ",";
  payload += "\"unidades\":\"" + unidades + "\"";
  payload += "}";

  // Publicar los datos en el tópico
  if (client.publish(mqtt_topic, payload.c_str())) {
    Serial.println("Datos publicados: " + payload);
  } else {
    Serial.println("Error al publicar los datos.");
  }
}

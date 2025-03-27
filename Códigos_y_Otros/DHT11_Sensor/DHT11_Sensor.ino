#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHT11 Sensor test!"));
  dht.begin();
}

void loop() {
  delay(5000); //Es un sensor lento, por lo que hay que darle tiempo.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float tt = dht.readTemperature(true);

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humedad: "));
  Serial.print(h);
  Serial.print(F("% Temperatura (°C): "));
  Serial.print(t);
  Serial.println(F("°C "));
  Serial.print(F("Temperatura (°F): "));
  Serial.print(tt);
  Serial.println(F("°F "));
}
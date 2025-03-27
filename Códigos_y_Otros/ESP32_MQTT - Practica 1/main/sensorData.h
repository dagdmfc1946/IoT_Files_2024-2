#ifndef SENSORDATA_H
#define SENSORDATA_H

// Definimos una estructura para almacenar los datos de temperatura y humedad
struct DatosSensor {
  float temperatura;
  float humedad;
};

// Función para generar un valor aleatorio de temperatura en el rango especificado
float generarTemperatura(int tempMin, int tempMax) {
  return random(tempMin * 10, tempMax * 10 + 1) / 10.0;
}

// Función para generar un valor aleatorio de humedad en el rango especificado
float generarHumedad(int humMin, int humMax) {
  return random(humMin * 10, humMax * 10 + 1) / 10.0;
}

// Función para generar y retornar los datos de temperatura y humedad, dependiendo del parámetro de entrada
DatosSensor generarDatosSensor(int parametro) {
  DatosSensor datos;

  // Variables para definir los rangos de temperatura y humedad
  int tempMin, tempMax, humMin, humMax;

  // Dependiendo del valor del parámetro de entrada, se definen los rangos
  switch (parametro) {
    case 1:
      tempMin = 10; tempMax = 15;
      humMin = 30; humMax = 40;
      break;
    case 2:
      tempMin = 15; tempMax = 20;
      humMin = 40; humMax = 50;
      break;
    case 3:
      tempMin = 20; tempMax = 25;
      humMin = 50; humMax = 60;
      break;
    case 4:
      tempMin = 25; tempMax = 30;
      humMin = 60; humMax = 70;
      break;
    case 5:
      tempMin = 30; tempMax = 35;
      humMin = 70; humMax = 80;
      break;
    case 6:
      tempMin = 35; tempMax = 40;
      humMin = 80; humMax = 90;
      break;
    case 7:
      tempMin = 40; tempMax = 45;
      humMin = 90; humMax = 100;
      break;
    default:
      // Valor por defecto si el parámetro está fuera del rango esperado
      tempMin = 20; tempMax = 25;
      humMin = 50; humMax = 60;
      break;
  }

  // Generamos los datos de temperatura y humedad con los rangos ajustados
  datos.temperatura = generarTemperatura(tempMin, tempMax);
  datos.humedad = generarHumedad(humMin, humMax);

  // Esperamos 1 segundo antes de generar nuevos datos
  delay(5000);

  return datos;  // Retorna la estructura con los valores
}

#endif


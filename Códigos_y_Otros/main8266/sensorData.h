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
    case 1: //PARIS
      tempMin = 11; tempMax = 15;
      break;
    case 2://BOGOTA
      tempMin = 15; tempMax = 20;
    
      break;
    case 3://B_AIRES
      tempMin = 20; tempMax = 25;
      
      break;
    case 4://MIAMI
      tempMin = 25; tempMax = 30;
      
      break;
    case 5://CANCUN
      tempMin = 30; tempMax = 33;
    
      break;
    case 6: //QATAR
      tempMin = 33; tempMax = 36;
      
      break;
    case 7: //SAHARA
      tempMin = 37; tempMax = 42;
      break;
    default:
      // Valor por defecto si el parámetro está fuera del rango esperado
      tempMin = 20; tempMax = 25;
      humMin = 50; humMax = 60;
      break;
  }

  // Generamos los datos de temperatura y humedad con los rangos ajustados
  datos.temperatura = generarTemperatura(tempMin, tempMax);
  

  // Esperamos 1 segundo antes de generar nuevos datos
  delay(5000);

  return datos;  // Retorna la estructura con los valores
}

#endif


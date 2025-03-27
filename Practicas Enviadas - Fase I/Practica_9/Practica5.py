import requests
import json
import time
from datetime import datetime
import uuid
import random

# Función simuladorSensor que genera un valor de temperatura aleatorio
def simuladorSensor():
    # Genera un valor de temperatura entre 15 y 25 grados Celsius
    temperatura = round(random.uniform(15.0, 25.0), 2)
    return temperatura

# URL del web service
webservice_url = "https://script.google.com/macros/s/AKfycbz7NCKBvRCvhQkuAjcaQUoxhpRQOD_4zOxplrXNP1s2fuiuB-CmPATKADO6ijK__Evm/exec"

# URL de la Google Sheet
sheet_url = "https://docs.google.com/spreadsheets/d/1dDH4QSKILZj-AcxiOz39hPtI8N9ac8EnHW9OVQCT62E/edit?gid=0#gid=0"

# Función principal que consume el web service
def registrar_datos():
    while True:
        # Generar la fecha actual y el valor de temperatura
        fecha = datetime.now().isoformat()
        temperatura = simuladorSensor()
        
        # Generar un ID único
        ID = str(uuid.uuid4())

        # Organizar el objeto de datos
        datos = {
            "ordentipo": "crear",
            "url": sheet_url,
            "numeroHoja": 0,
            "filaencabezados": 1,
            "columnaId": 2,
            "datos": json.dumps([
                fecha,
                ID,
                "Sensor001",
                "Laboratorio de Química",
                "Temperatura",
                "Celsius",
                temperatura
            ])
        }

        # Consumir el web service
        response = requests.post(webservice_url, json=datos)

        # Imprimir la respuesta del servidor
        print(response.text)

        # Esperar 5 segundos antes de la siguiente lectura
        time.sleep(5)

# Ejecutar la función principal
registrar_datos()

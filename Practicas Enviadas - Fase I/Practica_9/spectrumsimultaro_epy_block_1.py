import numpy as np
from gnuradio import gr
import json
import requests
from datetime import datetime

class blk(gr.sync_block):  # Heredando de gr.sync_block
    def __init__(self, url_google_sheet='https://docs.google.com/spreadsheets/d/1ZI1pO5DJjVnh0ph0jH8sRjnLGqXHOz-jBXJ1ZF875JM/edit?usp=sharing', 
                 latitud=7.141879, longitud=-73.122193, N=8, 
                 finicial=1417000000, fpaso=1421000000, salto=40000):
        gr.sync_block.__init__(self,
            name="Envia a Google Sheet",
            in_sig=[(np.float32, N)],  # Recibe un vector de tamaño N
            out_sig=None)  # No tiene salida
        
        # Parámetros externos
        self.url_google_sheet = url_google_sheet
        self.latitud = latitud
        self.longitud = longitud
        self.N = N
        self.finicial = finicial
        self.fpaso = fpaso
        self.salto = salto
        self.url_web_service = 'https://script.google.com/macros/s/AKfycbz7NCKBvRCvhQkuAjcaQUoxhpRQOD_4zOxplrXNP1s2fuiuB-CmPATKADO6ijK__Evm/exec'
        
        # Contador de ventanas espectrales procesadas
        self.entradas_procesadas = 0

    def registrarDatos(self, data):
        """
        Función que organiza y envía el JSON al servicio web.
        """
        # Convertir los datos del espectro a una cadena separada por comas
        data_str = ','.join([str(d) for d in data])
        
        # Obtener la fecha y hora actuales
        fecha_actual = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        
        # Crear el objeto JSON para la solicitud
        json_data = {
            "ordentipo": "crear",
            "url": self.url_google_sheet,  # URL de la Google Sheet
            "numeroHoja": 0,
            "filaencabezados": 1,
            "columnaId": 1,
            "datos": json.dumps([  # Envolviendo en json.dumps para convertirlo en una cadena JSON válida
                fecha_actual,
                self.latitud,  # Coordenada latitud
                self.longitud,  # Coordenada longitud
                "Medición de espectro",
                self.N,  # Número de muestras espectrales
                data_str,  # Datos espectrales
                self.finicial,  # Frecuencia inicial
                self.fpaso  # Paso de frecuencia
            ])
        }
        
        # Enviar el JSON como solicitud POST al servicio web
        try:
            response = requests.post(self.url_web_service, json=json_data)
            response.raise_for_status()  # Verificar que no haya errores
            print(f"Datos enviados exitosamente: {response.text}")
        except requests.exceptions.RequestException as e:
            print(f"Error enviando datos: {e}")

    def work(self, input_items, output_items):
        """
        Función principal que procesa las entradas y llama a la función de registro.
        """
        # Incrementar el contador de entradas procesadas
        self.entradas_procesadas += 1
        
        # Verificar si se han procesado suficientes entradas (definido por "salto")
        if self.entradas_procesadas >= self.salto:
            self.entradas_procesadas = 0  # Restablecer el contador
            
            # Recibir los datos del espectro (vector de tamaño N)
            data = input_items[0][0]
            
            # Registrar los datos en la Google Sheet
            self.registrarDatos(data)
        
        return len(input_items[0])  # Indica que se procesaron todas las muestras de entrada

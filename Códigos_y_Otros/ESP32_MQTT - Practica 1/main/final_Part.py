import paho.mqtt.client as mqtt
import json
import time

# llamada en caso de conexión exitosa
def al_conectarse(client, userdata, flags, rc):
    print('Se ha conectado al Broker MQTT y el resultado tiene código: ' + str(rc))
    # Suscribirse al topico
    client.subscribe('test_IoT_temperatura/')
    client.subscribe('test_IoT_humedad/')

# llamada en caso de recibir un mensaje
def al_llegar_mensaje(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))
    try:
        # Convertir el mensaje en formato JSON
        datos = json.loads(msg.payload.decode())
        temperatura = datos.get("temperatura")
        humedad = datos.get("humedad")

        # Guardar temperatura en un archivo
        with open("temperatura.txt", "a") as temp_file:
            temp_file.write(f"{time.strftime('%Y-%m-%d %H:%M:%S')} - Temperatura: {temperatura}°C\n")

        # Guardar humedad en un archivo
        with open("humedad.txt", "a") as hum_file:
            hum_file.write(f"{time.strftime('%Y-%m-%d %H:%M:%S')} - Humedad: {humedad}%\n")
    
    except Exception as e:
        print(f"Error al procesar el mensaje: {e}")

client = mqtt.Client("misegundaconeccion")

# Especificación para los callbacks
client.on_connect = al_conectarse
client.on_message = al_llegar_mensaje

# Establecimiento de una conexion
client.connect('broker.emqx.io', 1883, 60)

client.loop_forever()
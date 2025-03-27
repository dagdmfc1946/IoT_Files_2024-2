import paho.mqtt.client as mqtt
import time


# llamada en caso de conexión exitosa
def al_conectarse(client, userdata, flags, rc):
    print('Se ha conectado al Broker MQTT y el resultado tiene código: '+str(rc))
# Suscribirse al topico
    #client.subscribe('CASIRI/')
    client.subscribe('test_IoT_temperatura/')
    client.subscribe('test_IoT_humedad/')


#topic_hum = "C/humedad/"
#topic_tem = "C/temperatura/"

# llamada en caso de recibir un mensaje


def al_llegar_mensaje(client, userdata, msg):

    mensaje = msg.payload.decode('utf-8')
    print("Mensaje recibido: " + mensaje)

    # Extraer temperatura y humedad
    if "Temperatura" in mensaje and "Humedad" in mensaje:
        # Separar los valores
        partes = mensaje.split(',')
        # Extraer el valor de temperatura y humedad
        temperatura = partes[0].split(':')[1].strip().split(' ')[0]
        humedad = partes[1].split(':')[1].strip().split(' ')[0]

    print(f"Temperatura: {temperatura} °C")
    print(f"Humedad: {humedad} %")

    # Guardar los valores en archivos separados
    with open('temperatura.txt', 'a') as archivo_temp:
        archivo_temp.write(f"Temperatura: {temperatura} °C\n")

    with open('humedad.txt', 'a') as archivo_hum:
        archivo_hum.write(f"Humedad: {humedad} %\n")

client = mqtt.Client("misegundaconeccion")


# Especificación para los callbacks
client.on_message = al_llegar_mensaje


# Establecimiento de una conexion
client.connect('broker.emqx.io', 1883, 60)

cliente.loop_forever() 


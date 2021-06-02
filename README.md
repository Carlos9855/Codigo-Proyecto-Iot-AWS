# Codigo-Proyecto-Iot-AWS

***Uso de variables***

const char* WIFI_SSID:  es el nombre de la red

const char* WIFI_PASS: es la contraseña de la red

const char* AWS_ENDPOINT: es el endpoint propio de cada cuenta de AWS

const int AWS_ENDPOINT_PORT: es el puerto a través del cual se conecta AWS

int cm: variable para medir la distancia 
int estado: variable que determina si los LEDs se encienden (1), o se apagan (0)

const char* MQTT_CLIENT_ID: ID del cliente MQTT

const char* SUBCRIBE_TOPIC = "cato/iot/grupo8_in"; es el tópico al cual se debe suscribir para recibir mensajes

const char* PUBLISH_TOPIC = "cato/iot/grupo8_out"; // es el tópico al cual se debe suscribir para publicar mensajes
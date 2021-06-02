#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "TIGO-CAMACHO";
const char* WIFI_PASS = "2NB144200846";
const char* AWS_ENDPOINT = "a3qbsp8k2v8j5k-ats.iot.us-east-2.amazonaws.com";
const int AWS_ENDPOINT_PORT = 8883;
int cm = 0;
int estado=0;
const char* MQTT_CLIENT_ID = "YOUR_CLIENT_ID";
const char* SUBCRIBE_TOPIC = "cato/iot/grupo8_in"; // subscribe
const char* PUBLISH_TOPIC = "cato/iot/grupo8_out"; // publish

const char AMAZON_ROOT_CA1[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char CERTIFICATE[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUQtESjb+CGQMp9xBavYYXlVg44BwwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIxMDUxNzE0NTM1
NVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAK3yD8ZEd15BDnKwNyHE
/FzDEs/z6ruB1qMqpSHv91Lmr1S2qn0DoLzYNCFVHlmCm4G0/INNnOffoSQjyaBA
F857RLWfWnaExIz6wpCqNuF6SRbZP2jzxZkIzCnFU927Ph3MleRlvImIQPDHZ0KP
4WgeAWZtovt3n8JQOjDQM/XD+9V/VpcBcaBDtxusmxQZchsvHXM4RWkb610WBKPj
JwIS6rf4D9JMB0LXqNohhkhC/aOJOsYAbC1u1wR0pVQ7BaDjF7S21RaebXKirOLi
BvCgEYLBVjbdZ8irUxemBmfsfWE96Neh6ohS5qns3wj+u+w3F3DdPA0mD2eam8Yy
g/kCAwEAAaNgMF4wHwYDVR0jBBgwFoAUk2zy+04Kn87a5xv5Kytp4xxFSKIwHQYD
VR0OBBYEFER1u6l3BFQHRKSDXl3ZHp4fOa4iMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBTUrvbXIG0dkltfAVx+E4fK/3M
RpHXGBSQwhGzY0TOG0ALyk+lL9YKO9n06UdXQL0gde6kGE60CGTIpbfJV8LzFhdS
kc35q8SnHB9Dmox0Zj/pdb9KPJRDLNN3xKt7VceJyNJMM5H5vlX8e/+0O4d+NHM3
hrFIDnk19nMLhsRZUaCHfkdW2lwXVDoXNdsQ+ogn2bU1IuFhSGoJyOXOOf4a4TQb
H2l3iv+/1GpocYtjmrtj9PVEVk+DLkl5vKqBXvHgk3eASbjRXmEYAAk4DBqKJASn
7fFuKcKzaj5cdZttVzPVSFGmTOBeV9K04BhduX8bQc3VI/9qg4ouQJbfBbtl
-----END CERTIFICATE-----
)KEY";

const char PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEArfIPxkR3XkEOcrA3IcT8XMMSz/Pqu4HWoyqlIe/3UuavVLaq
fQOgvNg0IVUeWYKbgbT8g02c59+hJCPJoEAXzntEtZ9adoTEjPrCkKo24XpJFtk/
aPPFmQjMKcVT3bs+HcyV5GW8iYhA8MdnQo/haB4BZm2i+3efwlA6MNAz9cP71X9W
lwFxoEO3G6ybFBlyGy8dczhFaRvrXRYEo+MnAhLqt/gP0kwHQteo2iGGSEL9o4k6
xgBsLW7XBHSlVDsFoOMXtLbVFp5tcqKs4uIG8KARgsFWNt1nyKtTF6YGZ+x9YT3o
16HqiFLmqezfCP677DcXcN08DSYPZ5qbxjKD+QIDAQABAoIBAHH/rXTsVCryigyC
Dm/UUOxecIrXOeSPkGK5RSY7xaSQSSJE4PEzmnmJXIjn5w8UuOPGAKPxDr8sYch0
p+SdfLy3Lu21NS1tj/8xLKhw1azqXtJZqOQYt0y3Zk6N44vy9h/DgA0C3BBqutzS
YotODpUU7Sf6/JC/8nT/7VMmcJdFODblPiBg8QdC90draxk2d/oTX4precPCbl3X
Ytfqjhgv6L8MmxNUQ55/o+wUMIRLm/PzY4dKexSyxRrRVjsRIWgDgUqqdfPrGos4
qn3USJ0ijLnh9vktvxcIHN2vjVPxOnK04fTEC1yfBySiITDNHp6isGodIlhTQgQH
OWOgoq0CgYEA4LLW1elqfFoPaohAD4Bp1VHj/Yzw8Pf6Wbs0emeOvxSFgPdFUeYU
LbgX2icYKIJmzm4VfMA4qxTll5jrsAaEgoXdb3l0Nm1WOmfRrrYIlsK3tFQZqMtN
Mc26bG9qHex5pPSpFEh0texTfpeXap1kCh3fTZEUwxVYMwoFH+dQcP8CgYEAxi1G
1l2GWzYx7HhTDM3OqI37CeIsAb9h6lHXhcsfrODEf5doxKJnblX5snYS/RUT0ngu
butuCbV4oCz9u1NU0gA8tFkD8x9D8/Hdin05n2BUprSn5hh6iyv+L+Rvsmnf2D1h
Q5Pa+5DFq7LwGet68Q7X3mWFxjXiASgbWvDjkwcCgYEAtvIFJu8/yvVMnfFHLxff
k1Z0aWkKdzd/tvVL2elc+MiNlh49a1wHy92aVf0El17ss2A/0G109h4yfIZ+RrhX
xSmWy/84FmRZE36Z40TEo3LwPTtIFP1J4aXeBhGEdc6mkB6/VwtTMxeyN1zXZG+3
FuGRgUcBf4BuakfbOkmMXiMCgYEAsXBip5cPsGVi596IRcOUUJjnd975Fgt8qSfk
OBg54TlS1wcDOGRNBZ6XNrG/qLiRifYvZQMJAEpqmcgO2wXuwSSTNcsp3SpWVnpt
qQ0cZdYDlUvVtRCf0DJwogL/+5gOY3f5uV8OSAZE5soNbrfH2UQ4BhmSYHloRS7a
518d9EsCgYBYoErMO4MXsUPnBhQfWU2AWpnu7MsBQE4WkDhwO/L0GFNB5Viel21B
azQIgZ2ik6GFVGgAl6DhAiuuof2GSxET57dh/+RZI2r5JbbPdih6doc4KvsSOr4i
OXVHjS1LTv/4Iflci61WPfIoM3lZn9mklzwEwq+7YHoeZTmrWpKpPw==
-----END RSA PRIVATE KEY-----
)KEY";

WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

void ledOn() {
  estado=1;
  digitalWrite(LED_BUILTIN, HIGH);
}

void ledOff() {
  estado=0;
  digitalWrite(LED_BUILTIN, LOW);
}

StaticJsonDocument<JSON_OBJECT_SIZE(1)> inputDoc;

// PubSubClient callback function
void callback(const char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += String((char) payload[i]);
  }
  if (String(topic) == SUBCRIBE_TOPIC) {
    Serial.println("Message from topic " + String(SUBCRIBE_TOPIC) + ":" + message);

    DeserializationError err = deserializeJson(inputDoc, payload);
    if (!err) {
      String action = String(inputDoc["action"].as<char*>());
      if (action == "ON") ledOn();
      else if (action == "OFF") ledOff();
      else Serial.println("Unsupported action received: " + action);
    }
  }
}
void Led()
{
  if(estado==1)
  {
    if (cm > 200) 
  {
    digitalWrite(18, LOW);
    digitalWrite(19, LOW);
    digitalWrite(23, LOW);
    digitalWrite(5, LOW);
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
  }
  if (cm <= 200 && cm > 166) 
  {
    digitalWrite(18, HIGH);
    digitalWrite(19, LOW);
    digitalWrite(23, LOW);
    digitalWrite(5, LOW);
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
  }
  if (cm <= 166 && cm > 133) 
  {
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
    digitalWrite(23, LOW);
    digitalWrite(5, LOW);
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
  }
  if (cm <= 133 && cm > 100) 
  {
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
    digitalWrite(23, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
  }
  if (cm <= 100 && cm > 67)
  {
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
    digitalWrite(23, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
  }
  if (cm <= 67 && cm > 34)
  {
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
    digitalWrite(23, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(13, HIGH);
    digitalWrite(14, LOW);

  }
  if (cm <= 34) 
  {
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
    digitalWrite(23, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(13, HIGH);
    digitalWrite(14, HIGH);
  }
  }
  else
  {
    digitalWrite(18, LOW);
    digitalWrite(19, LOW);
    digitalWrite(23, LOW);
    digitalWrite(5, LOW);
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
   }
}
boolean mqttClientConnect() {
  Serial.println("Connecting to AWS " + String(AWS_ENDPOINT));
  if (mqttClient.connect(MQTT_CLIENT_ID)) {
    Serial.println("Connected to " + String(AWS_ENDPOINT));

    mqttClient.subscribe(SUBCRIBE_TOPIC);

    Serial.println("Subscribed to " + String(SUBCRIBE_TOPIC));
  } else {
    Serial.println("Can't connect to " + String(AWS_ENDPOINT));
  }
  return mqttClient.connected();
}

long readUltrasonicDistance(int triggerPin, int echoPin){
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(18, OUTPUT); //Puertos de salida
  pinMode(19, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  Serial.begin(115200);
  Serial.println("Connecting to " + String(WIFI_SSID));

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Can't connect to " + String(WIFI_SSID));
    while (1) delay (200);
  }
  Serial.println("Connected to " + String(WIFI_SSID));

  wifiClient.setCACert(AMAZON_ROOT_CA1);
  wifiClient.setCertificate(CERTIFICATE);
  wifiClient.setPrivateKey(PRIVATE_KEY);

  mqttClient.setServer(AWS_ENDPOINT, AWS_ENDPOINT_PORT);
  mqttClient.setCallback(callback);
}

unsigned long previousConnectMillis = 0;
unsigned long previousPublishMillis = 0;

unsigned char counter = 0;

StaticJsonDocument<JSON_OBJECT_SIZE(1)> outputDoc;
char outputBuffer[128];

void publishMessage(unsigned char value) {
  outputDoc["value"] = value;
  serializeJson(outputDoc, outputBuffer);
  mqttClient.publish(PUBLISH_TOPIC, outputBuffer);
}

void loop() {
    cm = 0.01723 * readUltrasonicDistance(14,27);
    Led();
  unsigned long now = millis();
  if (!mqttClient.connected()) {
    // Connect to MQTT broker
    if (now - previousConnectMillis >= 4000) {
      previousConnectMillis = now;
      if (mqttClientConnect()) {
        previousConnectMillis = 0;
      } else delay(1000);
    }
  } else {
    // This should be called regularly to allow the client to process incoming messages and maintain its connection to the server.
    mqttClient.loop();
    delay(20);

    if (now - previousPublishMillis >= 8000) {
      previousPublishMillis = now;
      // Publish message
      publishMessage(cm);
    }
  }
}

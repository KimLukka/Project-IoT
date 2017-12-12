#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "panoulu";
const char* password =  "";
const char* mqttServer = "m14.cloudmqtt.com";
const int mqttPort = 17353;
const char* mqttUser = "fbaeyadu";
const char* mqttPassword = "8IEoPV8J5yDI";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
 // client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("esp/test");
  client.publish("esp/test", "Awake activated");
  //client-publish("esp/test", 
  client.publish("esp/test", "Deep-Sleep activated");
  ESP.deepSleep(120e6); //e6 = seconds
  
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}

void loop() {
  client.loop();
}

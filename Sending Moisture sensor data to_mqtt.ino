#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "nijas2012";
const char* password = "urwombonye";
const char* mqtt_server = "broker.mqtt-dashboard.com";
WiFiClient espClient;
PubSubClient client(espClient); 
//Set-Up WiFi
void setup_wifi() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      client.publish("moisture","Reading Sensor Moisture Data:");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(6000);
    }
  }
} 
void callback(char* topic, byte* payload, unsigned int length) 
{
  } 

void setup() {
  Serial.begin(9600);

  Serial.println("Moisture test!");
  client.setServer(mqtt_server, 1883);
  reconnect();
}

void loop() {
  client.setCallback(callback);
  if (!client.connected()) {
  reconnect();
  }
  client.loop();
  
  int num=30.2;
  char cm[16];
  itoa(num,cm,10);
  Serial.println(cm);
  client.publish("SENSOR/TEMPERATURE",cm);
  delay(3000);
}

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTPIN D2     
#define DHTTYPE DHT22 
#define led D3 
DHT dht(DHTPIN, DHTTYPE);
const char* ssid = "nijas2012";
const char* password = "urwombonye";
#define TOKEN "A1E-BgFDEzhAYKGbZJ1q1dpWDS5W36Fklw"
#define MQTT_CLIENT_NAME "Janvier"
#define Variable_label "temperature"
#define Variable_label2 "humidity"
#define Variable_label_subscribe "relay"
#define device_label "home-automation"
//const char* mqtt_server = "broker.mqttdashboard.com";
const char* mqttbroker = "things.ubidots.com";
//const char* mqttbroker = "broker.mqttdashboard.com";
char payload[100];
char topic[50];
char topicSubscribe[150];
char sensor_data[10];
WiFiClient ubidots;
PubSubClient client(ubidots);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(MQTT_CLIENT_NAME,TOKEN,""))
    {
      Serial.println("connected");
      client.subscribe(topicSubscribe);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(1000);
    }
  }
} 
void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("MESSAGE ARRIVED: [");
  Serial.print("Subscribed to Topic: ");
  Serial.print(topic);
  
  int p =(char)payload[0]-'0';
  if(p==0) 
  {
    digitalWrite(led, LOW); 
    Serial.println(" Turning Off LED! ");
  } 
  if(p==1)
  {
    digitalWrite(led, HIGH); 
    Serial.println(" Turning On LED! ");
  }
  Serial.println();
}

void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(50);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void setup() {
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  pinMode(led,OUTPUT);
  setup_wifi();
  dht.begin();
  client.setServer(mqttbroker, 1883);
  reconnect();
  sprintf(topicSubscribe,"/v1.6/devices/%s/%s/Lv",device_label,Variable_label_subscribe);
  
}
void loop() {
  delay(1000);
  if (!client.connected()) {
  reconnect();
  }
  client.setCallback(callback);
  client.subscribe(topicSubscribe);
  client.loop();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  Serial.print("Publish a message Temp:");
  /*int numt=t;
  char cstr[16];
  itoa(numt,cstr,10);
Serial.println(cstr);*/
    String latbuf;
    latbuf += (String(t, 6));
    Serial.println(latbuf);
Serial.print("Publish a message Hum:");
  /*int numh=h;
  char cshr[16];
  itoa(numh,cshr,10);*/
   
 Serial.println("publishing data:");
 sprintf(topic,"%s%s","/V1.6/devices/",device_label);
 sprintf(payload,"%s","");
 sprintf(payload,"{\"%s\":",Variable_label);
 sprintf(payload,"%s{\"value\":%s}}",payload,latbuf);
 sprintf(payload,"%s{\"value\":%s}}",payload,latbuf);
 client.publish(topic, payload);
    String lontbuf;
    lontbuf += (String(h, 6));
    Serial.println(lontbuf);
 Serial.println(lontbuf);
 sprintf(payload,"%s","");
 sprintf(payload,"{\"%s\":",Variable_label2);
 sprintf(payload,"%s{\"value\":%s}}",payload,lontbuf);
 client.publish(topic, payload);
  delay(1000);
}

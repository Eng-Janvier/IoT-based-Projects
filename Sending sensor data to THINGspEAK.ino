#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <DHT.h>
#define DHTPIN D2     
#define DHTTYPE DHT22
const char* ssid = "ETOffice"; //Your Network SSID
const char* password = "EtT#3ecure@3"; //Your Network Password
 
WiFiClient client;
 
unsigned long myChannelNumber =914340; 
const char * myWriteAPIKey = "SJFRV8SCAQUA5VCL"; 
 
void setup()
 
{
 
Serial.begin(9600);
delay(10);
dht.begin();
// Connect to WiFi network 
WiFi.begin(ssid, password); 
ThingSpeak.begin(client); 
}
  
void loop()
{ 
float t = dht.readTemperature();
ThingSpeak.setField(1,t);
ThingSpeak.writeField(myChannelNumber, 1,t, myWriteAPIKey); //Update in ThingSpeak 
delay(1000);
 
}

#include "ESP8266WiFi.h"
#include <PubSubClient.h>
const char* ssid = "nijas2012"; //Enter SSID
const char* password = "urwombonye"; //Enter Password
const char* mqtt_server = "broker.mqttdashboard.com";
//const char* mqtt_server = "iot.eclipse.org";
WiFiClient espClient;
PubSubClient client(espClient);  
#include <TinyGPS.h>
#include <SoftwareSerial.h>
static const int RXPin = D4, TXPin = D3;
static const uint32_t GPSBaud = 9600;
int moisture;
// The TinyGPS++ object
TinyGPS gps;
float lat,lon;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);      
//yyyyyyyyyyyyyyyyyy
void callback(char* topic, byte* payload, unsigned int length) 
{
} //end callback
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("Doctor_message");
      //client.publish("Attendance","Janvier Detected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

void setup(void)
{ 
  Serial.begin(115200);
  ss.begin(GPSBaud);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(500);
     Serial.print("*");
  }
  
  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.println(WiFi.localIP());// Print the IP address
  client.setServer(mqtt_server, 1883);
  randomSeed(analogRead(0));
}
void loop() {
  if (!client.connected()) {
  reconnect();
  }
  client.loop();
  client.setCallback(callback);
  float acc_x = 0.000186;
  float acc_y = 0.000349;
  float acc_z = -0.000220;
  char buffer4[30];
  char buffer5[30];
  char buffer6[30];
  moisture = random(550, 600);
   while (ss.available() > 0){
    if (gps.encode(ss.read())){
    gps.f_get_position(&lat,&lon); // get latitude and longitude

    Serial.print("Position: ");
    
    //Latitude
    Serial.print("  Latitude:   ");
    Serial.print(lat,6);
    
    Serial.print(",  ");
    
    //Longitude
    Serial.print("Longitude: ");
    Serial.println(lon,6); 
    char buffer1[30];
    char buffer2[30];
    for (int i = 0; i < 7; i++) {
       dtostrf(lat, i+4, i, buffer1); 
       dtostrf(lon, i+4, i, buffer2);
       dtostrf(acc_x, i+4, i, buffer4); 
       dtostrf(acc_y, i+4, i, buffer5);
       dtostrf(acc_z, i+4, i, buffer6); 
       
   }
   char buffer3[16];
   Serial.print("   Water Level: ");
   Serial.println(moisture);
   Serial.print("   Accelerometer: ");
   Serial.print(" Acc_X:  ");
   Serial.print(buffer4);
   Serial.print(",  Acc_Y:   ");
   Serial.print(buffer5);
   Serial.print(", Acc_Z: ");
   Serial.println(buffer6);
   
    
   itoa(moisture,buffer3,10);
   client.publish("Sensors/Water/moisture",buffer3);
   client.publish("Sensors/gps/latitude",buffer1);
   client.publish("Sensors/gps/longitude",buffer2);
   client.publish("Sensors/accelerometer/X",buffer4);
   client.publish("Sensors/accelerometer/Y",buffer5);
   client.publish("Sensors/accelerometer/Z",buffer6);
   
   
   
  }
  }
   //delay(2002);
  }

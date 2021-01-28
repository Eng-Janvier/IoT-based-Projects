#include "MPU9250.h"
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
long speed1;
static const int RXPin = D4, TXPin = D3;
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPS gps;
float lat,lon;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
int sound_pin=A0;
int ad_value;
unsigned long start_time = 0;
unsigned long end_time = 0;
// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
int status;
const char* ssid = "nijas2012";
const char* password = "urwombonye";
const char* mqtt_server = "broker.mqtt-dashboard.com";
WiFiClient espClient;
PubSubClient client(espClient); 
//Set-Up WiFi
void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
//Connect MQTT
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("switch/fan");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} 
//Callback Function
void callback(char* topic, byte* payload, unsigned int length) 
{
    }
void setup()
{
  
  Serial.begin(115200);
  ss.begin(GPSBaud);
  pinMode(sound_pin,INPUT); 
  while(!Serial) {}

  // start communication with IMU 
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
    client.setServer(mqtt_server, 1883);
    setup_wifi();
    reconnect();
    delay(5000);
}

void loop()
{
  if (!client.connected()) {
  reconnect();
  }
  client.setCallback(callback);
  client.loop();
  speed1 = random(0, 6);
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0){
    if (gps.encode(ss.read())){
    gps.f_get_position(&lat,&lon); // get latitude and longitude

    Serial.print("Position: ");
    
    //Latitude
    Serial.print("Latitude: ");
    Serial.print(lat,6);
    char cstlat[16];
    itoa(lat,cstlat,10);
    client.publish("ADELINE/sensor/latitude",cstlat);
    Serial.print(",");
    char cstlon[16];
    itoa(lon,cstlon,10);
    client.publish("ADELINE/sensor/longitude",cstlon);
    //Longitude
    Serial.print("Longitude: ");
    Serial.println(lon,6); 
  }
  }
  ad_value=analogRead(sound_pin);
  Serial.print("Sound_wave:  ");
  char cstr[16];
  itoa(ad_value,cstr,10);
  client.publish("ADELINE/sensor/sound",cstr);
  Serial.print(ad_value);
  Serial.print("   Speed:  ");
  Serial.println(speed1);
  char cstp[16];
  itoa(speed1,cstp,10);
  client.publish("ADELINE/sensor/speeds",cstp);
  IMU.readSensor();
  Serial.print(" Acceleration measurement X: ");
  Serial.print(IMU.getGyroX_rads(),6);
  Serial.print("\t");
  Serial.print(" Y: ");
  Serial.print(IMU.getGyroY_rads(),6);
  Serial.print("\t");
  Serial.print(" Z: ");
  Serial.print(IMU.getGyroZ_rads(),6);
  Serial.print("\t");
  Serial.println();

  //PUBLISHING DATA
  char cstx[16];
  itoa(IMU.getGyroX_rads(),cstx,10);
  client.publish("ADELINE/sensor/acc-x",cstx);

  char csty[16];
  itoa(IMU.getGyroY_rads(),csty,10);
  client.publish("ADELINE/sensor/acc-y",csty);

  char cstz[16];
  itoa(IMU.getGyroZ_rads(),cstz,10);
  client.publish("ADELINE/sensor/acc-y",cstz);
  //delay(500);
  }

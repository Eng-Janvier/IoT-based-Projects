#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         5          
#define SS_PIN          15 
#define LED 4
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "nijas2012";
const char* password = "urwombonye";
const char* mqtt_server = "broker.mqttdashboard.com";
//const char* mqtt_server = "iot.eclipse.org";
WiFiClient espClient;
PubSubClient client(espClient);        
MFRC522 mfrc522(SS_PIN,RST_PIN);

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

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("REGULAR EMPLOYEES ATTENDANCE:");
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
client.setServer(mqtt_server, 1883);
}
void loop() {
  if (!client.connected()) {
  reconnect();
  }
  client.loop();
  client.setCallback(callback);
  if(mfrc522.PICC_IsNewCardPresent()){
          if(mfrc522.PICC_ReadCardSerial())
          {
            String content;
              for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //Serial.println(content);
 content.toUpperCase();
  //Serial.println();
  if (content.substring(1) == "E4 92 18 73") //check if student is jerome
  {
    Serial.print("NIYIGENA JEROME: ");
    Serial.println(content);
    client.publish("ATTENDANCE","NIYIGENA JEROME");
  }
  else if (content.substring(1) == "55 3D 7B 8D") 
  {
    Serial.print("UWIMANA ADELINE: ");
    Serial.println(content);
    client.publish("ATTENDANCE","UWIMANA ADELINE");
  }
   /*else if (content.substring(1) == "89 4B 50 14") 
  {
  Serial.print("NTIRENGANYA EMMANUEL: ");
  Serial.println(content);  
  client.publish("ATTENDANCE1","NTIRENGANYA EMMANUEL");
  }
   else if (content.substring(1) == "83 3B 9C 83") 
  {
  Serial.print("NIYITEGEKA JANVIER: ");
  Serial.println(content);  
  client.publish("ATTENDANCE","NIYITEGEKA JANVIER");
  }
  */
  else   {
   // Serial.println("EMPLOYEE NOT REGISTERED: ");
    delay(3000);
  }
            mfrc522.PICC_HaltA();
          }
        }else{
          Serial.println("Not detected");
        }
    delay(3000);
  }
  


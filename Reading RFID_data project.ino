#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define RST_PIN         9          
#define SS_PIN          10   
const char* ssid = "nijas2012";
const char* password = "urwombonye";
const char* mqtt_server = "broker.mqttdashboard.com";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
      
MFRC522 mfrc522(SS_PIN,RST_PIN);
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
void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker is : [");
  Serial.print("Subscribed to Topic: ");
  Serial.print(topic);
  Serial.print(" and ");
  int p =(char)payload[0]-'0';
  // if MQTT comes a 0 turn off LED on D2
  if(p==0) 
  {
    digitalWrite(BUZZER_PIN, LOW); 
    Serial.println(" Turning Off BUZZER! ");
  } 
  // if MQTT comes a 1, turn on BUZZER on pin D2
  if(p==1)
  {
    digitalWrite(BUZZER_PIN, HIGH); 
    Serial.println(" Turning On BUZZER! ");
  }
  Serial.println();
} //end callback
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
      client.subscribe("MQTT_Buzzer");
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
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID  reading UID");
  setup_wifi();
  client.setServer(mqtt_server, 1883);

}
void loop() {
  if(mfrc522.PICC_IsNewCardPresent()){
    //Serial.println("RFID DETECTED");
          if(mfrc522.PICC_ReadCardSerial())
          {
            Serial.print("Tag UID:");
            for(byte i=0; i< mfrc522.uid.size; i++)
            {
              Serial.print(mfrc522.uid.uidByte[i] < 0x10? " 0":" ");
              Serial.print(mfrc522.uid.uidByte[i],HEX);
            }
            Serial.println();
            mfrc522.PICC_HaltA();
          }
        }
  if (!client.connected()) {
  reconnect();
  }
  client.setCallback(callback);
  client.loop();

  }


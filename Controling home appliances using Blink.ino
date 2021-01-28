#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "6WNduBFNvwgloM0lZuxkLklbcSElYuPc";
char ssid[] = "nijas2012";
char pass[] = "urwombonye";
int led=D2;
BLYNK_WRITE(V1)
{
int pinValue = param.asInt(); 
Serial.print("DATA==");
Serial.println(pinValue);
if(pinValue==HIGH){
Serial.println("LED ON");
digitalWrite(led,HIGH);
}else{
 digitalWrite(led,LOW); 
}
}
void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(led,OUTPUT);
  Blynk.begin(auth,ssid,pass);
}

void loop()
{
  Blynk.run();
}

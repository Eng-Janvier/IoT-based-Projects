#include<SoftwareSerial.h>
SoftwareSerial ss(4,3);//Tx,Rx
int led1 = 7;
int led2 = 6;
int led3 = 5;
void setup() {
 pinMode(led1, OUTPUT);
 pinMode(led2, OUTPUT);
 pinMode(led3, OUTPUT);
 Serial.begin(9600); /* Define baud rate for serial communication */
 ss.begin(9600);
 Serial.println("Welcome");}
void loop() {
 
 if (ss.available())  /* If data is available on serial port */
    {
      Serial.println("Data Received");
      char data_received;
      
      data_received = ss.read();  /* Data received from bluetooth */
      if (data_received == 'A'){
       digitalWrite(led1, HIGH);
      Serial.println("LED1 turned ON");        }
      else{
      digitalWrite(led1, LOW);
      Serial.println("LED1 turned OFF");    
      }
      if (data_received == 'B')
      {
       digitalWrite(led2, HIGH);
       Serial.println("LED2 turned ON\n");
      }
       else{
      digitalWrite(led2, LOW);
      Serial.println("LED2 turned OFF");    
      }
      if(data_received == 'C')
      {
       digitalWrite(led3, HIGH);
       Serial.println("LED3 turned ON\n");
      }  
      else{
      digitalWrite(led3, LOW);
      Serial.println("LED3 turned OFF");    
      }
}}

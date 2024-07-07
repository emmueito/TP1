#include "mbed.h"

int sensorTemp;
float tempSen;
float temp;



void setup(){

    Serial.begin(9600);

 }

 void loop()
    {
  sensorTemp = analogRead(A0);
  tempSen = (sensorTemp * (5.0/1024));
  temp = (tempSen * 100)-55;
  Serial.println(temp, 1); // quiero un solo digito despeus de la coma
  delay(1000);
}

   


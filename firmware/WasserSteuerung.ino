/*
 * 17-Jan-2015
 * Christoph Kummer
 * 
 * WasserSteuerung / Version 1.1: 
 *      Diese Applikation soll mittels eines Sensors erkennen ob der Kanister bereist voll ist und 
 *      entsprechend das Magnetventil schliessen. Der Sensor besteht lediglich aus zwei Draehten  
 *      (eine Masse und der zweite am Analog/Digital-Wander angeschlossen). Weitere Informationen sind 
 *      im Wiki unter: https://github.com/chkummer/WasserSteuerung/wiki zu finden.
 */

#include <SoftwareSerial.h>

/* 
 * Pin definitionen 
 */
SoftwareSerial mySerial(4, 3); // RX, TX

/* Pin definitionen fuer Arduino Duemilanove (Entwicklung) */
 const byte waterValve = 10;
 const byte waterSensor = A0;

/* Pin definitionen fuer ATtiny (Produkt)
 const byte waterValve = 4;
 const byte waterSensor = A1;
*/

/*
 * Variablen
 */

//This is the average analog value found during startup. Usually ~995
//When hit with water, the analog value will drop to ~400. A diff of 100 is good.
int waterAvg = 0; 
int maxDifference = 400; //A diff of more than 100 in the analog value will trigger the system.

void setup()
{
  pinMode(waterValve, OUTPUT);

  pinMode(waterSensor, INPUT);
  //pinMode(waterSensor, INPUT_PULLUP);
  //pinMode(2, INPUT); //When setting the pin mode we have to use 2 instead of A1
  //digitalWrite(2, HIGH); //Hack for getting around INPUT_PULLUP

  mySerial.begin(9600);
  mySerial.println("WasserSteuerung/V1.1"); // Say hello
  digitalWrite(waterValve, LOW); //Turn off valve
  
  //Take a series of readings from the water sensor and average them
  waterAvg = 0;
  for(int x = 0 ; x < 8 ; x++)
  {
    waterAvg += analogRead(waterSensor);
    delay(100);
  }
  waterAvg /= 8;

  mySerial.print("Avg: ");
  mySerial.println(waterAvg);

  mySerial.println("ACTION: opening Valve");
  digitalWrite(waterValve, HIGH); //Turn on valve
}
 
void loop() 
{
  int waterDifference = abs(analogRead(waterSensor) - waterAvg);
  mySerial.print("Diff: ");
  mySerial.print(waterDifference);
  mySerial.print(" / Read: ");
  mySerial.println(analogRead(waterSensor));

  if(waterDifference > maxDifference) // Water detected turn off valve
  {
    long startTime = millis(); //Record the current time
    mySerial.println("ACTION: closing Valve");
    digitalWrite(waterValve, LOW); //Turn off valve

    //Loop until we don't detect water AND 2 seconds of alarm have completed
    while(waterDifference > maxDifference || (millis() - startTime) < 5000)
    {
      waterDifference = abs(analogRead(waterSensor) - waterAvg); //Take a new reading

      mySerial.print("Diff: ");
      mySerial.print(waterDifference);
      mySerial.print(" / Read: ");
      mySerial.println(analogRead(waterSensor));
      delay(1000);
    } //Loop until we don't detect water AND 2 seconds of alarm have completed

    mySerial.println("ACTION: opening Valve");
    digitalWrite(waterValve, HIGH); //Turn on valve
  }
  delay(1000);
}


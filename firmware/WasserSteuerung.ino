/*
 * 17-Jan-2015
 * Christoph Kummer
 * 
 * Wasser Steuerung: 
 *      Diese Applikation soll mittels eines Sensors erkennen ob der Kanister bereist voll ist und 
 *      entsprechend das Magnetventil schliessen. Der Sensor besteht lediglich aus zwei Draehten  
 *      (eine Masse und der zweite am Analog/Digital-Wander angeschlossen). Das Magnetventil stammt  
 *      von Play-Zone.ch (ROB-10456: http://www.play-zone.ch/de/magnetventil-solenoid-valve-12v-3-4.html). 
 *      Es ist im Normalzustand geschlossen, es oeffnet sich sobald am Magnetventil eine Spannung 
 *      von 12VDC zugefuert wird. Die Grundidee stammt vom H2OhNo-Projekt von SparkFun (Produkt-Seite: 
 *      https://www.sparkfun.com/products/12069 / Projekt-Seite: https://github.com/sparkfun/H2OhNo).  
 *      Fuer die Entwickung (Prototyp) wurde ein Arduino Duemilanove verwendet. Im Produkt soll ein 
 *      ATtiny85 (oder ATtiny45 je nach Verfuegbarkeit) zum Einsatz kommen. Um den ATtiny zu Programmieren 
 *      kann man nach den folgenden 'High-Low Tech' Anleitungen verfahren: 'Programming an ATtiny w/ 
 *      Arduino' (http://highlowtech.org/?p=1695) resp. 'Arduino board as ATtiny programmer' 
 *      (http://highlowtech.org/?p=1706). Dazu muss die Arduino IDE mit der ATtiny Unterstuetzung  
 *      (https://github.com/damellis/attiny) ausgestattet werden. 
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
  mySerial.println("WasserSteuerung"); // Say hello
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

  digitalWrite(waterValve, HIGH); //Turn on valve
}
 
void loop() 
{
  int waterDifference = abs(analogRead(waterSensor) - waterAvg);
  mySerial.print("Diff: ");
  mySerial.println(waterDifference);

  if(waterDifference > maxDifference) // Water detected turn off valve
  {
    long startTime = millis(); //Record the current time
    digitalWrite(waterValve, LOW); //Turn off valve

    //Loop until we don't detect water AND 2 seconds of alarm have completed
    while(waterDifference > maxDifference || (millis() - startTime) < 5000)
    {
      waterDifference = abs(analogRead(waterSensor) - waterAvg); //Take a new reading

      mySerial.print("Read: ");
      mySerial.println(analogRead(waterSensor));
      delay(1000);
    } //Loop until we don't detect water AND 2 seconds of alarm have completed

    digitalWrite(waterValve, HIGH); //Turn on valve
  }
  delay(1000);
}


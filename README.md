# WasserSteuerung
Mittels eines Sensors ein Magnetventils den Wasserfluss steuern (Kanister voll = stop).

Diese Applikation soll mittels eines Sensors erkennen ob der Kanister bereist voll ist und 
entsprechend das Magnetventil schliessen. Der Sensor besteht lediglich aus zwei Draehten  
(eine Masse und der zweite am Analog/Digital-Wander angeschlossen). Das Magnetventil stammt  
von Play-Zone.ch (ROB-10456: http://www.play-zone.ch/de/magnetventil-solenoid-valve-12v-3-4.html). 
Es ist im Normalzustand geschlossen, es oeffnet sich sobald am Magnetventil eine Spannung 
von 12VDC zugefuert wird. Die Grundidee stammt vom H2OhNo-Projekt von SparkFun (Produkt-Seite: 
https://www.sparkfun.com/products/12069 / Projekt-Seite: https://github.com/sparkfun/H2OhNo).  
Fuer die Entwickung (Prototyp) wurde ein Arduino Duemilanove verwendet. Im Produkt soll ein 
ATtiny85 (oder ATtiny45 je nach Verfuegbarkeit) zum Einsatz kommen. Um den ATtiny zu Programmieren 
kann man nach den folgenden 'High-Low Tech' Anleitungen verfahren: 'Programming an ATtiny w/ 
Arduino' (http://highlowtech.org/?p=1695) resp. 'Arduino board as ATtiny programmer' 
(http://highlowtech.org/?p=1706). Dazu muss die Arduino IDE mit der ATtiny Unterstuetzung  
(https://github.com/damellis/attiny) ausgestattet werden. 

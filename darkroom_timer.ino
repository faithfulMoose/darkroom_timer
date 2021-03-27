#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);


const int startButtonPin = 4;
const int tenUpButtonPin = 5;
const int oneUpButtonPin = 6;
const int sharpButtonPin = 7;
const int lampRelayPin = 8;

int counter = 0;
int burnTime = 0;
int lampRelayState = LOW;
int startButtonState;
int startButtonReading;
int lastStartButtonState = LOW;
int tenUpButtonState;
int tenUpButtonReading;
int lastTenUpButtonState = LOW;
int oneUpButtonState;
int lastOneUpButtonState = LOW;
int oneUpButtonReading;
int sharpButtonState;
int lastSharpButtonState = LOW;
int sharpButtonReading;
int i;
const int myDelay = 978;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;


void setup() {
  // put your setup code here, to run once:
  pinMode(startButtonPin, INPUT);
  pinMode(tenUpButtonPin, INPUT);
  pinMode(oneUpButtonPin, INPUT);
  pinMode(sharpButtonPin, INPUT);
  pinMode(lampRelayPin, OUTPUT);
  digitalWrite(lampRelayPin, lampRelayState);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  display.setBrightness(1);
 

// bij druk op knop telt de counter 1 seconde bij:

  oneUpButtonReading = digitalRead(oneUpButtonPin);

  if (oneUpButtonReading != lastOneUpButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (oneUpButtonReading != oneUpButtonState) {
      oneUpButtonState = oneUpButtonReading;

      if (oneUpButtonState == HIGH) {
        counter += 1;
      }
    }
  }

// bij druk op knop telt counter 10 seconden bij:

  tenUpButtonReading = digitalRead(tenUpButtonPin);

  if (tenUpButtonReading != lastTenUpButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (tenUpButtonReading != tenUpButtonState) {
      tenUpButtonState = tenUpButtonReading;

      if (tenUpButtonState == HIGH) {
        counter += 10;
      }
    }
  } 

// Bij druk op start gaat lamp aan gedurende de ingestelde tijd (counter) 

  startButtonReading = digitalRead(startButtonPin);
 
  
  if (startButtonReading != lastStartButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
cf    if (startButtonReading != startButtonState) {
      startButtonState = startButtonReading;

      if (startButtonState == HIGH) {          
        //while(burnTime > 0) { 
          for(i=0; i<counter; i++){  
            digitalWrite(lampRelayPin, HIGH);                                            
            burnTime --;
            display.showNumberDec(burnTime, false); //display toont het aftellen 
            delay(myDelay);
            //Serial.println(burnTime);
          }
       digitalWrite (lampRelayPin, LOW); //Na de while loop wordt de lamp terug uitgeschakeld
       //display.showNumberDec(0, false);
       delay (1000);
       }
    }
  } 

  // Bij druk op de scherpstelknop brandt de lamp, pas bij een tweede druk gaat ze uit.
  
  sharpButtonReading = digitalRead(sharpButtonPin);

  if (sharpButtonReading != lastSharpButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (sharpButtonReading != sharpButtonState) {
      sharpButtonState = sharpButtonReading;

      if (sharpButtonState == HIGH) {
        lampRelayState = !lampRelayState;
      }
    }
  }

 // Bij Samen indrukken +1 en +10 reset counter

  if ((oneUpButtonState == HIGH) && (tenUpButtonState == HIGH)) {
    counter = 0; 
  }

 
  burnTime = counter;
  display.showNumberDec(burnTime, false);
  digitalWrite(lampRelayPin, lampRelayState);
  
  lastOneUpButtonState = oneUpButtonReading;
  lastTenUpButtonState = tenUpButtonReading;
  lastStartButtonState = startButtonReading;
  lastSharpButtonState = sharpButtonReading;
  

  

}

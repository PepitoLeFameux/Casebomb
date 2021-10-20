#include <Wire.h>                                            // appel des bibliotheques
#include <Arduino.h>
#include "rgb_lcd.h"

rgb_lcd lcd;
// const int LED_PIN = 8;
const int INTERRUPT_PIN = 3;
volatile bool ledState = LOW; // on assigne une valeur booléenne variable pour l'état de la LED, qui prendra comme valeur HIGH ou LOW   

unsigned long previousMillis = 0;
unsigned long temps_ms; 
const unsigned long interval = 5000; // constante à 10000ms = 10s

void setup() {
    interrupts();   
    lcd.begin(16,2);
    lcd.setRGB(127, 127, 127);
    lcd.setCursor(0,0);
    // Serial. begin(9600);
    // pinMode(LED_PIN, OUTPUT);
    // pinMode(INTERRUPT_PIN, INPUT_PULLUP);
    // attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), myISR, CHANGE); //marche pas pour notre arduino
    for (int i = 0; i< 98; i ++) {
        pinMode(i, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(i), myISR, RISING); //marche pas pour notre arduino
    }
}

void loop() {
    temps_ms=millis();
    
    // digitalWrite(LED_PIN, ledState);
    
    if(temps_ms - previousMillis >= interval) {
        previousMillis = temps_ms;
        //   Serial.println("Loop"); // Affiche Loop sur le moniteur série toutes les constantes (10s)
        lcd.print("Loop"); // Affiche Loop sur le moniteur série toutes les constantes (10s)
    }
}

void myISR() { // Interruption
    delay(1000);
    ledState = !ledState;
    lcd.print("Interrupt"); 
}
// note : LOW == false == 0, HIGH == true == 1, donc inverser le booléen revient à commuter entre LOW et HIGH.

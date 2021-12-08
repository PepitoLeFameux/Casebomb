
#include <Wire.h> // appel des bibliotheques
#include "rgb_lcd.h"

int SlaveReceived = 0;
int SlaveSend = 5;

int colorR = 0;
int colorG = 255;
int colorB = 0;

unsigned long tempsDepart;
unsigned long temps;
unsigned long tempsEcoule;
unsigned long tempsMax = 300000; //300000 ms = 5 min
int secondeAvant = 0;

bool fini = false;

int minutes;
int secondes;
int led = 9;

int depart = 0;
rgb_lcd lcd;

bool LEDs = false;
int NSA;
int MSA;
int FRK;
int ledNSA=11;
int ledMSA=12;
int ledFRK=13;
extern int *list_value_LED[] = {&NSA, &MSA, &FRK};

void receiveEvent(int combien) { //Fonction appelée lorsque l'esclave recoit un signal
    SlaveReceived = Wire.read();
    // Serial.println("received data " + SlaveReceived);
    Serial.println("received data:");
    Serial.println(SlaveReceived);
}

void requestEvent() { // fonction appelée lorsque l'esclave envoie un signal
    Wire.write(SlaveSend);
    //Serial.println("sending data:");
    //Serial.println(SlaveSend);
}

void setup() {
    Wire.begin(8);
    Wire.setClock(100000);
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    lcd.setCursor(0, 0);
    lcd.print("Compte a rebours");
    //pinMode(A0,INPUT);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}

void loop() {
    
    if (fini != true) {
    if(SlaveReceived>0 && LEDs == false){
      NSA = SlaveReceived % 2 + 1;
      MSA = SlaveReceived % 3 + 1;
      FRK = SlaveReceived % 5 + 1;      
      if (NSA==1){digitalWrite(ledNSA,HIGH);}
      Serial.println("NSA");
      Serial.println(NSA);
      Serial.println("MSA");
      Serial.println(MSA);
      Serial.println("FRK");
      Serial.println(FRK);
      if (MSA==1){digitalWrite(ledMSA,HIGH);}
      if (FRK==1){digitalWrite(ledFRK,HIGH);}
      LEDs = true;
    }


    
    if (SlaveReceived == 50 && fini == false && LEDs == true) {
        if (depart == 0) {
            tempsDepart = millis();
            depart = 1;
        }
        temps = millis();
        tempsEcoule = tempsMax - (temps - tempsDepart);
        minutes = tempsEcoule / 60000; //minutes écoulées avec 60000 ms
        secondes = (tempsEcoule - minutes * 60000) / 1000;

        lcd.setCursor(0, 1);
        if (minutes > 9) {
            lcd.print(minutes);
        } else {
            lcd.print(0);
            lcd.print(minutes);
        }
        lcd.print(":");
        if (secondes > 9) {
            lcd.print(secondes);
            lcd.print("                       ");
        } else {
            lcd.print(0);
            lcd.print(secondes);
            lcd.print("                       ");

            
        }

        if (!(colorG == 0 && colorR == 255) && secondeAvant != secondes) {
            colorG = colorG - 2;
            colorR = colorR + 2;
            lcd.setRGB(colorR, colorG, colorB);
            secondeAvant = secondes;
        }

        if (minutes == 0 && secondes == 0) {
            fini = true;
            Serial.println("timer off");
            SlaveSend = 10;
        }
    }
    
    //Valeur recue est 20 si victoire de la part de la maitre,10 pour une défaite et 50 pour départ chrono
    //Valeur envoyée 10 quand chrono fini
    if ((SlaveReceived == 10 || fini == true )&& LEDs == true) { //affiche message de defaite
        Serial.println("game end");
        //SlaveSend = 10;
        lcd.setRGB(255, 0, 0);
        lcd.setCursor(0, 0);
        lcd.print("      BOUM      ");
        lcd.setCursor(0, 1);
        lcd.print("                 ");
        delay(1000);
    }

    if (SlaveReceived == 20 && LEDs == true) {
        Serial.println("game victory");
        lcd.setRGB(50, 200, 50);
        lcd.setCursor(0, 0);
        lcd.print("Temps restant :      ");
        //lcd.setCursor(0, 1);
        //lcd.print("  Felicitation   ");
    }
    }
    delay(100);
}

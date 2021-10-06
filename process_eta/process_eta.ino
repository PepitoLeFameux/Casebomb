#include <Wire.h>                                            // appel des bibliotheques
#include "rgb_lcd.h"
#define A10 , A11

//ecran lcd
rgb_lcd lcd; 

//assignation des pins des composants
int bouton = A8;
//cables
int cable1 = A0;// 2.50V
int cable2 = A1;// 3.00V
int cable3 = A2;// 4.10V
int cable4 = A3;// 4.50V
int cable5 = A4;// 4.70V
int list_cables[] = [cable1, cable2, cable3, cable4, cable5];
// LED
int ledNSA = 11;// pinledNSA
int ledMSA = 10;
int ledFRK = 9;
// arduino
int ardui_in = 53;
int ardui_out = 51;
int ardui_clock = 40;

// // valeur des 3 LED
// int NSA;// ledNSA
// int MSA;
// int FRK;
void init_lcd() {
    // initialisation lcd avec test
    lcd.begin(16,2);
    lcd.setRGB(colorR, colorG, colorB);
    lcd.setCursor(0,0);
  //debug si tu veux
    lcd.print("amogus");
}

void init_leds() {
    //génération aléatoire d'un seed grâce à la tension instable au bornes d'un pin
    randomSeed(analogRead(9));
    NSA = random()%2;
    MSA = random()%2;
    FRK = random()%2;
    pinMode(ledNSA,OUTPUT);
    pinMode(ledMSA,OUTPUT);
    pinMode(ledFRK,OUTPUT);

  //allume les LEDs qui sont activées
    if (NSA == 1) { analogWrite(ledNSA,30); }
    if (MSA == 1) { analogWrite(ledMSA,30); }
    if (FRK == 1) { analogWrite(ledFRK,30); }
}

void init_pins() {
    
    //communication foireuse entre les arduinos
    pinMode(ardui_out,OUTPUT);
    pinMode(ardui_in,INPUT);
    
    pinMode(bouton,INPUT);
    pinMode(cable1,INPUT);
    pinMode(cable2,INPUT);
    pinMode(cable3,INPUT);
    pinMode(cable4,INPUT);
    pinMode(cable5,INPUT);
}



void setup() {
    // randomSeed(analogRead(9));
    //initialisation du system général
    // I2C ?
    //initialisation des modules principaux (times et autres si nécessaire)

    init_lcd();
    init_leds();
    init_pins()

    
    creecode();   
    gencombinaison();
    
  //envoie un signal à l'arduino 2
    digitalWrite(ardui_out,HIGH);
    delay(10);
    digitalWrite(ardui_out,LOW);
    
    //affiche le code erreur
    lcd.print(code);
    lcd.setCursor(0,1);
    //affiche la combinaison correcte (debug)
    for (int i = 0; i < 5; i ++) {
        lcd.print(combinaison[i]);
    }

    //la partie se joue tant que erreur<3 et pas victoire
    while (erreur < 3 && victoire == 0) {

        //attente de l'appui du bouton (maintenu)
        while (analogRead(bouton) < 800) {
        
            //vérifie en boucle que l'arduino 2 n'a pas envoyé de signal de fin de timer
            perdu = digitalRead(ardui_clock);
            if (perdu == 1) { break; }
        }

        //si jamais le joueur a perdu, quite la boucle
        if (perdu == 1) { break; }

        // assignation des valeurs de chaque port en fonction du branchement des cables 
        lcd.setCursor(0,1);
        for (int i = 0; i < 5; i ++) {
            //list_cables[i]
            float voltage = analogRead(list_cables[i]) * 5.0 / 1023.0;//mesure le voltage de chaque port 
            resultats[i] = numero(voltage);//assigne un numéro au port en fonction du voltage du cables qui lui est branché
            lcd.print(combinaison[i]);//affiche les résultats attendus //debug
        }

        //place le curseur  juste après le code
        lcd.setCursor(6,1);

        //cherche si la combinaison rentrée comporte une erreur
        if (checkErreur() == 1) { erreur++; }
        else { victoire = 1; }
        
        void lcd_sur_2ligne(int pos, [char] msg) {
            for (int i = 0; i < 2; i ++) {
                lcd.setCursor(pos, i);
                for (int j = 0; j < 2; j ++) {
                        lcd.print(msg);
                }
            }
        }

        if (erreur == 1) {// le joueur se trompe 1 fois
            lcd.setRGB(255, 171, 0);//lcd en "jaune"
            //affiche une grosse croix
            lcd_sur_2ligne(11, "X");
        }
        if (erreur == 2) {// le joueur se trompe 2 fois
            lcd.setRGB(226, 53, 0);//lcd en "rouge"
            //affiche une deuxième grosse croix
            lcd_sur_2ligne(14, "X");
        }

        //attend le relâchement du bouton pour continuer
        while (analogRead(bouton) > 900) {
        
            //vérifie en boucle que l'arudino 2 n'a pas envoyé de signal de fin de timer
            perdu = digitalRead(ardui_clock);
            if (perdu == 1) { break; }
        }
    }
    /*
void printlcd((int,int) pose, ([char], [char]) characts) {
    lcd.setCursor(pose.0, pose.1);
    for(int i = 0; i > charats.len(); i++) {
        lcd.print(characts[i]);
    } 
}
*/

    //en fin de partie, si perdu(compte à rebours) ou 3 erreurs
    if (erreur == 3 || perdu == 1) {
        lcd.setRGB(0, 0, 0);//lcd en "noir"
        lcd.setCursor(0,0);
        lcd.print("     Perdu      ");
        lcd.setCursor(0,1);
        lcd.print(" Recommencer ?  ");
    }
    
  //sinon victoire
    else {
        lcd.setCursor(0,0);
        lcd.print("  Desamorcage   ");
        lcd.setCursor(0,1);
        lcd.print("    Reussi      ");
    }


  //coupe le signal envoyé vers l'arduino 2
    digitalWrite(ardui_out,LOW);
}

void Module1() {
    
}
void Module2() {
    
}
void Module3() {
    
}
void Module4() {
    
}
void Module5() {
    
}
void Module6() {
    
}

int state = 0;
bool module_finished = false;

int last_seed = -1;
void loop() {
    // generate a seed to choose which function to launch
    int seed = random(10) % 6;
    while (seed == last_seed) {
        seed = random(10) % 6;
    }
    while (!module_finished) {
        switch (seed) {
            case 0 : {
                Module1();
                break;
            }
            case 1 :{
                Module2();
                break;
            }
            case 2 : {
                Module3();
                break;
            }
            case 3 : {
                Module4();
                break;
            }
            case 4 : {
                Module4();
                break;
            }
            case 5 : {
                Module4();
                break;
            }
            default: { break; }
        }
    }
    last_seed = seed;
    module_finished = false;
}

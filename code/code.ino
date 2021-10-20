#include <Wire.h>
#include <Arduino.h>
#include "rgb_lcd.h"
//#include "header.h"

#define A10 , A11


//// pins
//cables
const int cable1 = A0;// 2.50V
const int cable2 = A1;// 3.00V
const int cable3 = A2;// 4.10V
const int cable4 = A3;// 4.50V
const int cable5 = A4;// 4.70V
const int *list_cables[] = {&cable1, &cable2, &cable3, &cable4, &cable5};
// LEDs
const int ledNSA = 11;// pinledNSA
const int ledMSA = 10;
const int ledFRK = 9;
const int *list_LEDs[] = {&ledNSA, &ledMSA, &ledFRK};
// arduino
const int ardui_in = 53;
const int ardui_out = 51;
const int ardui_clock = 40;
const int *list_ardui[] = {&ardui_in, &ardui_out, &ardui_clock};
//bouton
const int bouton = 2;
//ecran lcd
rgb_lcd lcd;


//// valeurs des pins
// LEDs
int NSA;
int MSA;
int FRK;
int *list_value_LED[] = {&NSA, &MSA, &FRK};// [value_NSA, value_MSA, value_FRK]
// bouton
int button_state = false;


//// autres
// initialisation des variables
int resultats[] = {0,0,0,0,0}; //combinaison rentrée
int combinaison[] = {0,0,0,0,0}; //combinaison correcte

int lchiffres[] = {0,0,0,0,0,0,0,0};
int llettres[] = {0,0,0,0,0,0,0,0};
int chiffres;
int lettres;

bool module_finished = false;
char code[] = "00000000";

//variables d'etat du jeu
int erreur = 0;
int victoire = 0;
int perdu = 0;
// int state = 0;

int last_seed = -1;

void button_press() {
    button_state = !button_state;
    lcd.print("bouton");
}
void setup() {
    // interrupts();   
    lcd.begin(16,2);
    lcd.setRGB(50,50,50);
    lcd.setCursor(0,0);
    //génération aléatoire d'un seed grâce à la tension instable au bornes d'un pin
    randomSeed(analogRead(9));}
    // attachInterrupt(digitalPinToInterrupt(interruptPin), button_press, CHANGE);

void loop() {
    // generate a seed to choose which function to launch
    // int seed = random(10) % 6;
    // while (seed == last_seed) {
    //     seed = random(10) % 6;
    // }
    int seed = 0;
    while (!module_finished) {
        switch (seed) {
            case 0 : {
                Module1(combinaison, llettres, lchiffres, lettres, chiffres);
                break;
            }
            // case 1 :{
            //     Module2();
            //     break;
            // }
            // case 2 : {
            //     Module3();
            //     break;
            // }
            // case 3 : {
            //     Module4();
            //     break;
            // }
            // case 4 : {
            //     Module4();
            //     break;
            // }
            // case 5 : {
            //     Module4();
            //     break;
            // }
            default: { break; }
        }
    }
    last_seed = seed;
    module_finished = false;
}

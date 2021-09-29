
#include <Wire.h>                                            // appel des bibliotheques
#include "rgb_lcd.h"
#define A10 , A11
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

//utilite ? / c'est quoi ?
int colorR = 255;                                    
int colorG = 255;
int colorB = 255;

//ecran lcd
rgb_lcd lcd; 

//pin des cables
int bouton = A8;
//cable1 2.50V
//cable2 3.00V
//cable3 4.10V
//cable4 4.50V
//cable5 4.70V
int cable1 = A0;
int cable2 = A1;
int cable3 = A2;
int cable4 = A3;
int cable5 = A4;

float V1,V2,V3,V4,V5 = 0;

//ordre des câbles à brancher
//affiche 42069 si ok sinon 666
unsigned int OK = 42069;

//variables d'etat du jeu
int erreur = 0;
int victoire = 0;
int perdu = 0;

int numero(float tension) {
    if (tension < 0.1) { return 0; }
    if (tension < 0.3) { return 5; }
    if (tension < 0.7) { return 4; }
    if (tension < 1.5) { return 3; }
    if (tension < 2.0) { return 2; }
    if (tension < 3.0) { return 1; }
    else { return 9; }
}

//int i;
//int n;

// initialisation des variables
int resultats[] = {0,0,0,0,0};
int combinaison[] = {0,0,0,0,0};

int lchiffres[] = {0,0,0,0,0,0,0,0};
int lpair[] = {0,0,0,0,0,0,0,0};
int limpair[] = {0,0,0,0,0,0,0,0};
int impair;//? pas utilise
int pair;//? pas utilise
int chiffres;//? pas utilise

int llettres[] = {0,0,0,0,0,0,0,0};
int lvoyelles[] = {0,0,0,0,0,0,0,0};
int lconsonnes[] = {0,0,0,0,0,0,0,0};
int lettres;
int voyelles;//? pas utilise
int consonnes;//? pas utilise

char code[] = "00000000";
// valeur des 3 LED
int NSA;
int MSA;
int FRK;
// pin des 3 LED
int ledNSA = 11;
int ledMSA = 10;
int ledFRK = 9;

int checkErreur() {
    for(i = 0; i < 5; i++) {
        if (resultats[i] != combinaison[i]) { return 1; }
    }
    return 0;
}

int n;
void creecode(){
    for (i = 0; i < 8; i++){
        n = random()%36;
        code[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"[n];
  
        if (0 <= n && n <= 25) {
            llettres[i] = 1;
            lettres++;
    
            if (n == 0 || n == 4 || n == 8 || n == 14 || n == 20 || n == 24){
                lvoyelles[i] = 1;
                voyelles++;
            } else {
                lconsonnes[i] = 1;
                consonnes++;
            }
        } else {
            lchiffres[i] = 1;
            chiffres++;
            if (n%2 == 0) {
                lpair[i] = 1;
                pair++;
            } else {
                limpair[i] = 1;
                impair++;
            }
        }
    }
}

int condE4(){
    if (pasbranche(5) == 1) { return 5; }
    if (pasbranche(1) == 1) { return 1; }
    else { return 0; }
}

// possibillite de completement enleve les "un","deux",...
int chiffreE4(){
    int un=0;
    int deux=0;
    int trois=0;
    int quatre=0;
    int cinq=0;
    for(i=0;i<5;i++) {
        if (combinaison[i]==1) {un=1;}
        else if (combinaison[i]==2) {deux=1;}
        else if (combinaison[i]==3) {trois=1;}
        else if (combinaison[i]==4) {quatre=1;}
        else if (combinaison[i]==5) {cinq=1;}
    }
    if(un==0){ return 1; }
    if(deux==0){ return 2; }
    if(trois==0){ return 3; }
    if(quatre==0){ return 4; }
    if(cinq==0){ return 5; }
}

int remplace_chiffreE4() {
    for (i = 0; i < 5; i++) {
        if      (combinaison[i] == 1) { return 1; }
        else if (combinaison[i] == 2) { return 2; }
        else if (combinaison[i] == 3) { return 3; }
        else if (combinaison[i] == 4) { return 4; }
        else if (combinaison[i] == 5) { return 5; }
    }
}

int dans(char num[], char liste[]){
    for (i = 0; i < 8; i++) {
      if (liste[i] == num[0]) { return 1; }
    }
    return 0;
}

int cdans(int num, int liste[]) {
    for (i = 0; i < 8; i++){
        if (liste[i] == num) { return 1; }
    }
    return 0;
}

int pasbranche(int n) {
    if (cdans(n,combinaison) == 1) { return 0; }
    else { return 1; }
}

int addition() {
    int add = 0;
    for (i = 0; i < 5; i++) {
        if (combinaison[i] == 0) { add = add + i + 1; }
    }
}

int libre() {
    int lib = 0;
    for (i = 0; i < 5; i++) {
        if (combinaison[i] == 0) { lib++; }
    }
    return lib;
}
              
void gencombinaison() {
    if (lchiffres[2] == 1 ) { combinaison[0] = 4; }
    else if (NSA == 1 && llettres[6] == 1 ) { combinaison[0] = 3; }
    else if (dans("2",code) == 1 ) { combinaison[0] = 5; }
    else { combinaison[0] = 4; }
  
    if (MSA == 1) { combinaison[1]=1 ;}
    else if (lettres >= 4 && pasbranche(3) == 1 ) { combinaison[1] = 3; }
    else if (dans("2",code) == 1 && dans("4",code) == 1 && pasbranche(5) == 1) { combinaison[1] = 5; }
    else { combinaison[1] = 2; }

    if (pasbranche(5) == 1 && dans("V",code)) { combinaison[2] = 5; }
    else if (pasbranche(1) == 1 && lettres >= 5) { combinaison[2] = 1; }
    else if (llettres[3] == "1" && lchiffres[6] == "1" && pasbranche(2) == 1) { combinaison[2] = 2; }
    else if (pasbranche(3) == 1) { combinaison[2] = 3; }
    else { combinaison[2] = 0; }

    if (dans("D",code) && dans("E",code) && pasbranche(4) == 1) { combinaison[3] = 4; }
    else if (FRK == 1 && MSA == 1 && pasbranche(2) == 1) { combinaison[3] = 2; }
    else if (addition() >= 7 && pasbranche(1) == 1) { combinaison[3] = 1; }
    else if (dans("0",code) == 1 && dans("5",code) == 1 && pasbranche(5)) { combinaison[3] = 5; }
    else { combinaison[3] = 0; }

    if (libre() > 2) { combinaison[4] = 0; }
    else if (cdans(1,combinaison) == 1 && cdans(3,combinaison) == 1 && cdans(5,combinaison) == 1) { combinaison[4] = 4; }
    else if (condE4() == 1 || condE4() == 5 ) { combinaison[4] = condE4(); }
    else if (dans("B",code) == 1 && dans("P",code) == 1) { combinaison[4] = chiffreE4(); }
}

// fonction qui s'execute au lancement du programme
void setup() {  
    lcd.begin(16,2);
    lcd.setRGB(colorR, colorG, colorB);
    lcd.setCursor(0,0);
    lcd.print("amogus");
    
    randomSeed(analogRead(9));
    NSA = random()%2;
    MSA = random()%2;
    FRK = random()%2;
    pinMode(ledNSA,OUTPUT);
    pinMode(ledMSA,OUTPUT);
    pinMode(ledFRK,OUTPUT);
    
    if (NSA == 1) { analogWrite(ledNSA,30); }
    if (MSA == 1) { analogWrite(ledMSA,30); }
    if (FRK == 1) { analogWrite(ledFRK,30); }
  
    pinMode(51,OUTPUT);
    pinMode(53,INPUT);
    
    creecode();   
    gencombinaison();
    digitalWrite(51,HIGH);
    delay(10);
    digitalWrite(51,LOW);
    
    pinMode(bouton,INPUT);
    pinMode(cable1,INPUT);
    pinMode(cable2,INPUT);
    pinMode(cable3,INPUT);
    pinMode(cable4,INPUT);
    pinMode(cable5,INPUT);
  
    lcd.print(code);
    lcd.setCursor(0,1);
    lcd.print(combinaison[0]);
    lcd.print(combinaison[1]);
    lcd.print(combinaison[2]);
    lcd.print(combinaison[3]);
    lcd.print(combinaison[4]);

    while (erreur < 3 && victoire == 0) {
        while (analogRead(bouton) < 800) {
            perdu = digitalRead(40);
            if (perdu == 1) { break; }
        }
  
        if (perdu == 1) { break; }
        float V1 = analogRead(cable1)*5.0/1023.0;
        float V2 = analogRead(cable2)*5.0/1023.0;
        float V3 = analogRead(cable3)*5.0/1023.0;
        float V4 = analogRead(cable4)*5.0/1023.0;
        float V5 = analogRead(cable5)*5.0/1023.0;
        resultats[0] = numero(V1);
        resultats[1] = numero(V2);
        resultats[2] = numero(V3);
        resultats[3] = numero(V4);
        resultats[4] = numero(V5);
        
        lcd.setCursor(0,1);
        lcd.print(combinaison[0]);
        lcd.print(combinaison[1]);
        lcd.print(combinaison[2]);
        lcd.print(combinaison[3]);
        lcd.print(combinaison[4]);
        
        lcd.setCursor(6,1);
  
        if (checkErreur() == 1) { erreur++; }
        else { victoire = 1; }
        
        if (erreur == 1) {
          colorR = 255;
          colorG = 171;
          colorB = 0;
          lcd.setRGB(colorR, colorG, colorB);
          lcd.setCursor(11,0);
          lcd.print("X");
          lcd.print("X");
          lcd.setCursor(11,1);
          lcd.print("X");
          lcd.print("X");
        }
        if (erreur == 2) {
            colorR = 226;
            colorG = 53;
            colorB = 0;
            lcd.setRGB(colorR, colorG, colorB);
            lcd.setCursor(14,0);
            lcd.print("X");
            lcd.print("X");
            lcd.setCursor(14,1);
            lcd.print("X");
            lcd.print("X");
        }
        while (analogRead(bouton) > 900) {
            perdu = digitalRead(40);
            if (perdu == 1) { break; }
        }
    }
    if (erreur == 3 || perdu == 1) {
        colorR = 0;
        colorG = 0;
        colorB = 0;
        lcd.setRGB(colorR, colorG, colorB);// pourquoi ne pas juste mettre 0 partout ? 
        lcd.setCursor(0,0);
        lcd.print("     Perdu      ");
        lcd.setCursor(0,1);
        lcd.print(" Recommencer ?  ");
    } else {
        lcd.setCursor(0,0);
        lcd.print("  Desamorcage   ");
        lcd.setCursor(0,1);
        lcd.print("    Reussi      ");
    }
    
    digitalWrite(51,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
}

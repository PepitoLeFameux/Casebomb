
#include <Wire.h>                                            // appel des bibliotheques
#include "rgb_lcd.h"
#define A10 , A11
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

//couleur écran code (pour la modifier en cours de route éventuellement
int colorR = 80;                                    
int colorG = 80;
int colorB = 80;

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

// permet d'associer une tension au numéro du cable branché
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
int resultats[] = {0,0,0,0,0}; //combinaison rentrée
int combinaison[] = {0,0,0,0,0}; //combinaison correcte

// les arrays comme lpair représentent la position des nombres impairs dans le code
// par exemple lvoyelles[]={0,0,1,0} indique un une voyalle en 3eme position dans le code
int lchiffres[] = {0,0,0,0,0,0,0,0};
int lpair[] = {0,0,0,0,0,0,0,0};
int limpair[] = {0,0,0,0,0,0,0,0};
int impair;
int pair;
int chiffres;

int llettres[] = {0,0,0,0,0,0,0,0};
int lvoyelles[] = {0,0,0,0,0,0,0,0};
int lconsonnes[] = {0,0,0,0,0,0,0,0};
int lettres;
int voyelles;
int consonnes;

char code[] = "00000000";
// valeur des 3 LED
int NSA;
int MSA;
int FRK;
// pin des 3 LED
int ledNSA = 11;
int ledMSA = 10;
int ledFRK = 9;

//compare la combinaison correcte à la combinaison rentrée, renvoie 1 en cas d'erreur, 0 sinon
int checkErreur() {
    for(int i = 0; i < 5; i++) {
        if (resultats[i] != combinaison[i]) { return 1; }
    }
    return 0;
}

//génère le code et remplit les critères comme le nombre de voyelles, les nombres impairs...
int n;
void creecode(){
    for (int i = 0; i < 8; i++){
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
    for(int i=0;i<5;i++) {
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
    for (int i = 0; i < 5; i++) {
        if      (combinaison[i] == 1) { return 1; }
        else if (combinaison[i] == 2) { return 2; }
        else if (combinaison[i] == 3) { return 3; }
        else if (combinaison[i] == 4) { return 4; }
        else if (combinaison[i] == 5) { return 5; }
    }
}

//cherche la présence d'un caractère dans une liste
int dans(char lettre[], char liste[]){
    for (int i = 0; i < 8; i++) {
      if (liste[i] == lettre[0]) { return 1; }
    }
    return 0;
}

//cherche la présence d'un chiffre dans une liste
int cdans(int num, int liste[]) {
    for (int i = 0; i < 8; i++){
        if (liste[i] == num) { return 1; }
    }
    return 0;
}

//renvoie 1 si le cable n n'est pas branché
int pasbranche(int n) {
    if (cdans(n,combinaison) == 1) { return 0; }
    else { return 1; }
}

// ?????????
int addition() {
    int add = 0;
    for (int i = 0; i < 5; i++) {
        if (combinaison[i] == 0) { add = add + i + 1; }
    }
}

int libre() {
    int lib = 0;
    for (int i = 0; i < 5; i++) {
        if (combinaison[i] == 0) { lib++; }
    }
    return lib;
}

//la combinaison correcte est générée grâce aux conditions aléatoires et aux règles du jeu
void gencombinaison() {
      //CABLE A
    //si chiffre en 3eme position -> branche au 4
    if (lchiffres[2] == 1 ) { combinaison[0] = 4; }
    //si NSA allumée et lettre en 7eme position -> branche au 3
    else if (NSA == 1 || FRK == 1 && llettres[6] == 1 ) { combinaison[0] = 3; }
    //si le code comporte un 2 -> branche au 5
    else if (dans("2",code) == 1 ) { combinaison[0] = 5; }
    //sinon branche au 4
    else { combinaison[0] = 4; }

      //CABLE B
    //si MSA allumée -> branche au 1
    if (MSA == 1) { combinaison[1]=1 ;}
    //si plus de 3 lettres et port 3 vide -> branche au 3    
    else if (lettres > 3 && pasbranche(3) == 1 ) { combinaison[1] = 3; }
    //si le code comporte 2 et 4 et que port 5 vide -> branche au 5
    else if (dans("2",code) == 1 && dans("4",code) == 1 && pasbranche(5) == 1) { combinaison[1] = 5; }
    //sinon branche au 2
    else { combinaison[1] = 2; }

      //CABLE C
    //si port 5 vide et le code comporte un V -> branche au 5
    if (pasbranche(5) == 1 && dans("V",code)) { combinaison[2] = 5; }
    //si port 1 vide et plus de 4 lettres dans le code -> branche au 1
    else if (pasbranche(1) == 1 && lettres > 4) { combinaison[2] = 1; }
    //si lettre en 4eme position et chiffre en 7eme position et port 2 vide -> branche au 2
    else if (llettres[3] == "1" && lchiffres[6] == "1" && pasbranche(2) == 1) { combinaison[2] = 2; }
    //si port 3 vide -> branche au 3
    else if (pasbranche(3) == 1) { combinaison[2] = 3; }
    //sinon ne pas brancher
    else { combinaison[2] = 0; }

      //CABLE D
    //si le code comporte un D, un E et que le port 4 est vide -> branche au 4
    if (dans("D",code) || dans("E",code) || dans("T",code) && pasbranche(4) == 1) { combinaison[3] = 4; }
    //si FRK et MSA allumé et port 2 vide -> branche au 2
    else if (FRK == 1 && pasbranche(2) == 1) { combinaison[3] = 2; }
    //si l'addition des numéros des ports vides est supérieure à 6 et port 1 vide -> branche au 1
    else if (addition() > 6 && pasbranche(1) == 1) { combinaison[3] = 1; }
    //si le code comporte un 0 (zéro) et un 5 et port 5 vide -> branche au 5
    else if (dans("0",code) == 1 && dans("5",code) == 1 && pasbranche(5)) { combinaison[3] = 5; }
    //sinon ne pas brancher
    else { combinaison[3] = 0; }

      //CABLE E
    //si plus de 1 port vides -> ne branche pas
    if (libre() > 1) { combinaison[4] = 0; }
    //si les ports 1,3,5 sont occupés -> branche au 4
    else if (cdans(1,combinaison) == 1 && cdans(3,combinaison) == 1 && cdans(5,combinaison) == 1) { combinaison[4] = 4; }
    //si le port 5 est vide -> branche au 5, sinon si le 1 est vide -> branche au 1
    else if (condE4() == 1 || condE4() == 5 ) { combinaison[4] = condE4(); }
    //si le code comporte un B et un P -> branche au dernier port (le seul pas branché)
    else if (dans("B",code) == 1 && dans("P",code) == 1) { combinaison[4] = chiffreE4(); }
}


// fonction qui s'execute au lancement du programme
void setup() {
    Serial.begin(9600);
    lcd.begin(16,2);
    lcd.setRGB(colorR, colorG, colorB);
    lcd.setCursor(0,0);

  //génération aléatoire d'un site grâce à la tension instable au bornes d'un pin
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

  //communication foireuse entre les arduinos
    pinMode(51,OUTPUT);
    pinMode(53,INPUT);
    
    creecode();   
    gencombinaison();
    
  //envoie un signal à l'arduino 2
    digitalWrite(51,HIGH);
    delay(10);
    digitalWrite(51,LOW);
    
    pinMode(bouton,INPUT);
    pinMode(cable1,INPUT);
    pinMode(cable2,INPUT);
    pinMode(cable3,INPUT);
    pinMode(cable4,INPUT);
    pinMode(cable5,INPUT);

  //affiche le code erreur
    lcd.print(code);
    lcd.setCursor(0,1);
  //affiche la combinaison correcte (debug)
    lcd.print(combinaison[0]);
    lcd.print(combinaison[1]);
    lcd.print(combinaison[2]);
    lcd.print(combinaison[3]);
    lcd.print(combinaison[4]);

  //la partie se joue tant que erreur<3 et pas victoire
    while (erreur < 3 && victoire == 0) {
        // le joueur intéragie avec les cables 
        // il appui pour "confirmer" son/ses choix
    //attente de l'appui du bouton (maintenu)
        while (analogRead(bouton) < 800) {
          
          //vérifie en boucle que l'arudino 2 n'a pas envoyé de signal
            perdu = digitalRead(40);
            if (perdu == 1) { break; }
        }

    //si jamais le joueur a perdu, quiite la boucle
        if (perdu == 1) { break; }

    //mesure la tension aux bornes de chaque cable
        float V1 = analogRead(cable1)*5.0/1023.0;
        float V2 = analogRead(cable2)*5.0/1023.0;
        float V3 = analogRead(cable3)*5.0/1023.0;
        float V4 = analogRead(cable4)*5.0/1023.0;
        float V5 = analogRead(cable5)*5.0/1023.0;

    //et leur attribue un numéro
        resultats[0] = numero(V1);
        resultats[1] = numero(V2);
        resultats[2] = numero(V3);
        resultats[3] = numero(V4);
        resultats[4] = numero(V5);


    //affiche la combinaison encore (debug)
        lcd.setCursor(0,1);
        lcd.print(combinaison[0]);
        lcd.print(combinaison[1]);
        lcd.print(combinaison[2]);
        lcd.print(combinaison[3]);
        lcd.print(combinaison[4]);

     //place le curseur  juste après le code
        lcd.setCursor(6,1);

     //cherche si la combinaison rentrée comporte une erreur
        lcd.setCursor(9,0);
        lcd.print(resultats[0]);
        lcd.print(resultats[1]);
        lcd.print(resultats[2]);
        lcd.print(resultats[3]);
        lcd.print(resultats[4]);
        Serial.println(V1);
        Serial.println(V2);
        Serial.println(V3);
        Serial.println(V4);
        Serial.println(V5);
        
        if (checkErreur() == 1) { erreur++; }
        else { victoire = 1; }
        
        if (erreur == 1) {
        //la couleur vire vers le rouge
          colorR = 127;
          colorG = 85;
          colorB = 0;
          lcd.setRGB(colorR, colorG, colorB);
        //affiche une grosse croix
          lcd.setCursor(11,0);
          //lcd.print("X");
          //lcd.print("X");
          lcd.setCursor(11,1);
          lcd.print("X");
          lcd.print("X");
        }
        if (erreur == 2) {
        //la couleur vire encore
            colorR = 127;
            colorG = 26;
            colorB = 0;
            lcd.setRGB(colorR, colorG, colorB);
        //affiche une deuxième grosse croix
            lcd.setCursor(14,0);
            //lcd.print("X");
            //lcd.print("X");
            lcd.setCursor(14,1);
            lcd.print("X");
            lcd.print("X");
        }

      //attend le relâchement du bouton pour continuer
        while (analogRead(bouton) > 900) {

          //vérifie en boucle que l'arudino 2 n'a pas envoyé de signal
            perdu = digitalRead(40);
            if (perdu == 1) { break; }
        }
    }


  //en fin de partie, si perdu(comtpe à rebours) ou 3 erreurs
    if (erreur == 3 || perdu == 1) {
        colorR = 0;
        colorG = 0;
        colorB = 0;
        lcd.setRGB(colorR, colorG, colorB);// pourquoi ne pas juste mettre 0 partout ? 
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
    digitalWrite(51,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
}

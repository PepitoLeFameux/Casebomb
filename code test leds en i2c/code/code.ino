#include <Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define A10 , A11

//// pins
//cables
extern const int cable1 = A0;// 2.50V
extern const int cable2 = A1;// 3.00V
extern const int cable3 = A2;// 4.10V
extern const int cable4 = A3;// 4.50V
extern const int cable5 = A4;// 4.70V
//const int *list_cables[] = {&cable1, &cable2, &cable3, &cable4, &cable5};
extern const int list_cables[] = {A0, A1, A2, A3, A4};
// LEDs
extern const int ledNSA = 11;// pinledNSA
extern const int ledMSA = 10;
extern const int ledFRK = 9;
extern const int *list_LEDs[] = {&ledNSA, &ledMSA, &ledFRK};
// arduino
//const int ardui_in = 53;
//const int ardui_out = 51;
//const int ardui_clock = 40;
//const int *list_ardui[] = {&ardui_in, &ardui_out, &ardui_clock};
//bouton
extern const int bouton = 2;


//ecran lcd
extern LiquidCrystal_I2C lcd(0x27, 20, 4);


//// valeurs des pins
// LEDs
int NSA;
int MSA;
int FRK;
extern int *list_value_LED[] = {&NSA, &MSA, &FRK};// [value_NSA, value_MSA, value_FRK]
// bouton
extern int button_state = false;


//// autres
// initialisation des variables
extern int resultats[] = {0,0,0,0,0}; //combinaison rentrée
extern int combinaison[] = {0,0,0,0,0}; //combinaison correcte

extern int lchiffres[] = {0,0,0,0,0,0,0,0};
extern int llettres[] = {0,0,0,0,0,0,0,0};
extern int chiffres;
int lettres;

extern bool module_finished = false;
extern char code[] = "00000000";

//variables d'etat du jeu
extern int erreur = 0;
extern int victoire = 0;
extern int perduTemps = 0;
// int state = 0;

int last_seed = -1;

void button_press() {
    button_state = !button_state;
    //lcd.print("bouton");// DO NOT USE
}



void init_lcd() {
    // initialisation lcd avec test
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
}




//PARTIE I2C

extern int MasterSend = 0;
extern int MasterReceive = 0;

//void receiveEvent(int dfgh){
//  MasterReceive=Wire.read();
//}

//void checkChrono(){
//  Wire.requestFrom(8,1);
//  perduTemps = Wire.read();    // receive a byte as character
//  Serial.println(perduTemps);         // print the character
//}

void checkChrono(){
    Wire.requestFrom(8, 6);    // request 6 bytes from slave device #8
    //Serial.println("trying to receive data");
    while (Wire.available()) { // slave may send less than requested
        int c = Wire.read(); // receive a byte as character
        //Serial.println("Master, received data:");
        //Serial.println(c);
    }

//   delay(500);
//   Wire.requestFrom(8, 1);    // request 8 bytes from slave device #8
  
//   perduTemps = Wire.read(); // receive a byte as character
//   Serial.println(Wire.available());         // print the character
//   Serial.println(perduTemps);         // print the character
//   delay(100);
}





void setup() {
// interrupts();  
    init_lcd();
    init_leds();
    Serial.begin(9600);

    //communication des LEDs avec la Uno : 111 si les 3 sont allumées, 101 si NSA et FRK sont allumées
    MasterSend=1;
    Serial.println("hello");
    Serial.println(MasterSend);
    if(NSA==1){MasterSend*= 2;}
    Serial.println(MasterSend);
    if(MSA==1){MasterSend*= 3;}
    Serial.println(MasterSend);
    if(FRK==1){MasterSend*= 5;}
    Serial.println(MasterSend);
    
    Wire.beginTransmission(8);
    Wire.write(MasterSend);
    Wire.endTransmission();
        
    //génération aléatoire d'un seed grâce à la tension instable au bornes d'un pin
    randomSeed(analogRead(7));
    attachInterrupt(digitalPinToInterrupt(bouton), button_press, CHANGE);
    
    //PARTIE I2C
    Wire.begin();
    Wire.setClock(100000);
    delay(3000);
    MasterSend=50; //50 = partie commence
    Wire.beginTransmission(8);
    Wire.write(MasterSend);
    Wire.endTransmission();
    Serial.println("sending:");
    Serial.println(MasterSend);
}



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
    if (victoire == 1) {
        MasterSend = 20; //20 = partie gagnée
        Serial.print("victoire");
        Wire.beginTransmission(8);
        Wire.write(MasterSend);
        Wire.endTransmission();
        Serial.println("sending:");
        Serial.println(MasterSend);
    }
    last_seed = seed;
    module_finished = false;
}

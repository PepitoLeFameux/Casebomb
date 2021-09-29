// #include <Wire.h>                                            // appel des bibliotheques
// #include "rgb_lcd.h"

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

void setup() {
    randomSeed(analogRead(9));
    //initialisation du system général
    // I2C ?
    //initialisation des modules principaux (times et autres si nécessaire)
}

int state = 0;
bool module_finished = false;

int last_seed = -1;
void loop() {
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
    state++;
}

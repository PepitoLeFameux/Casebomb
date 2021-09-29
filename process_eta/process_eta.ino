#include <Wire.h>                                            // appel des bibliotheques
#include "rgb_lcd.h"

void setup() {
  //initialisation du system général
  // I2C ?
  //initialisation des modules principaux (times et autres si nécessaire)
}

int state = 0;
bool module_finished = false;

void loop() {
    int seed = random() % 3;
    while (!module_finished) {
        switch (seed) {
            case 0 : Module1();
                break;
            case 0 : Module1();
                break;
            case 0 : Module1();
                break;
            case 0 : Module1();
                break;
        }
    }
    module_finished = false;
    state++;
}

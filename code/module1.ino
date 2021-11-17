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

void lcd_sur_2ligne(int pos, char msg[]) {
    for (int i = 0; i < 2; i ++) {
        lcd.setCursor(pos, i);
        for (int j = 0; j < 2; j ++) {
                lcd.print(msg);
        }
    }
}

//compare la combinaison correcte à la combinaison rentrée, renvoie 1 en cas d'erreur, 0 sinon
int checkErreur() {
    for(int i = 0; i < 5; i++) {
        if (resultats[i] != combinaison[i]) { return 1; }
    }
    return 0;
}

void creecode(int *llettres[], int *lchiffres[], int *lettres, int *chiffres){
    for (int i = 0; i < 8; i++){
        int n = random()%36;
        code[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"[n];
        if (0 <= n && n <= 25) {
            *llettres[i] = 1;
            *lettres++;
        } else {
            *lchiffres[i] = 1;
            *chiffres++;
        }
    }
}

int condE4(){
    if (pasbranche(5) == 1) { return 5; }
    if (pasbranche(1) == 1) { return 1; }
    else { return 0; }
}

int chiffreE4() {
    for (int i = 0; i < 5; i++) {
        if      (combinaison[i] == 1) { return 1; }
        else if (combinaison[i] == 2) { return 2; }
        else if (combinaison[i] == 3) { return 3; }
        else if (combinaison[i] == 4) { return 4; }
        else if (combinaison[i] == 5) { return 5; }
    }
}


void gencombinaison(int combinaison[], int llettres[], int lchiffres[]) {
      //CABLE A
    //si chiffre en 3eme position -> branche au 4
    if (lchiffres[2] == 1 ) { combinaison[0] = 4; }
    //si NSA allumée et lettre en 7eme position -> branche au 3
    else if (NSA == 1 || FRK == 1 && llettres[6] == 1 ) { combinaison[0] = 3; }
    //si le code comporte un 2 -> branche au 5
    else if (dans("2", code) == 1 ) { combinaison[0] = 5; }
    //sinon branche au 4
    else { combinaison[0] = 4; }

      //CABLE B
    //si MSA allumée -> branche au 1
    if (MSA == 1) { combinaison[1]=1 ;}
    //si plus de 3 lettres et port 3 vide -> branche au 3    
    else if (lettres > 3 && pasbranche(3) == 1 ) { combinaison[1] = 3; }
    //si le code comporte 2 et 4 et que port 5 vide -> branche au 5
    else if (dans("2", code) == 1 && dans("4", code) == 1 && pasbranche(5) == 1) { combinaison[1] = 5; }
    //sinon branche au 2
    else { combinaison[1] = 2; }

      //CABLE C
    //si port 5 vide et le code comporte un V -> branche au 5
    if (pasbranche(5) == 1 && dans("V", code)) { combinaison[2] = 5; }
    //si port 1 vide et plus de 4 lettres dans le code -> branche au 1
    else if (pasbranche(1) == 1 && lettres > 4) { combinaison[2] = 1; }
    //si lettre en 4eme position et chiffre en 7eme position et port 2 vide -> branche au 2
    else if (llettres[3] == 1 && lchiffres[6] == 1 && pasbranche(2) == 1) { combinaison[2] = 2; }
    //si port 3 vide -> branche au 3
    else if (pasbranche(3) == 1) { combinaison[2] = 3; }
    //sinon ne pas brancher
    else { combinaison[2] = 0; }

      //CABLE D
    //si le code comporte un D, un E et que le port 4 est vide -> branche au 4
    if (dans("D", code) || dans("E", code) || dans("T", code) && pasbranche(4) == 1) { combinaison[3] = 4; }
    //si FRK et MSA allumé et port 2 vide -> branche au 2
    else if (FRK == 1 && pasbranche(2) == 1) { combinaison[3] = 2; }
    //si l'addition des numéros des ports vides est supérieure à 6 et port 1 vide -> branche au 1
    else if (addition() > 6 && pasbranche(1) == 1) { combinaison[3] = 1; }
    //si le code comporte un 0 (zéro) et un 5 et port 5 vide -> branche au 5
    else if (dans("0", code) == 1 && dans("5", code) == 1 && pasbranche(5)) { combinaison[3] = 5; }
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
    else if (dans("B", code) == 1 && dans("P", code) == 1) { combinaison[4] = chiffreE4(); }
}

void lcd_clean() {
    for (int i = 0; i < 1; i ++) {
        for (int j = 0; j < 16; j ++){
            lcd.setCursor(j,i);
            lcd.print(" ");
        }
    }
}

void button_pressed() {
    //si jamais le joueur a perdu, ne rien faire
    if (perduTemps != 1) {
        lcd_clean();
        // assignation des valeurs de chaque port en fonction du branchement des cables 
        for (int i = 0; i < 5; i ++) {
            float voltage = analogRead(list_cables[i]) * 5.0 / 1023.0;// mesure le voltage de chaque port 
            resultats[i] = numero(voltage);// assigne un numéro au port en fonction du voltage du cables qui lui est branché
            lcd.setCursor(i,0);
            lcd.print(combinaison[i]);// affiche la combinaison voulu //debug
            lcd.setCursor(i,1);
            lcd.print(resultats[i]);// affiche les résultats lut //debug
        }

        //place le curseur  juste après le code
        lcd.setCursor(6,1);

        //cherche si la combinaison rentrée comporte une erreur
        if (checkErreur() == 1) { erreur++; }
        else { victoire = 1; }
        

        if (erreur == 1) {// le joueur se trompe 1 fois
            //affiche une grosse croix
            lcd_sur_2ligne(11, "X");
        }
        if (erreur == 2) {// le joueur se trompe 2 fois
            //affiche une deuxième grosse croix
            lcd_sur_2ligne(14, "X");
        }
    }
}


void Module1(int combinaison[], int llettres[], int lchiffres[], int lettres, int chiffres) {

    init_pins();
    init_leds();

    creecode(&llettres, &lchiffres, &lettres, &chiffres);   
    // gencombinaison(combinaison, llettres, lchiffres);
    for(int i = 1 ; i < 6; i ++) {
      combinaison[i] = 0;
    }
    // combinaison = [1, 2, 3, 4, 5];

    //envoie un signal à l'arduino 2
    //digitalWrite(ardui_out, HIGH);
    //delay(10);
    //digitalWrite(ardui_out, LOW);
    
    //affiche le code erreur
    lcd.setCursor(0,0);
    lcd.print(code);
    lcd.setCursor(0,1);
    //affiche la combinaison correcte (debug)
    for (int i = 0; i < 5; i ++) {
        lcd.print(combinaison[i]);
        //lcd.print(DEBUG[i]);
        //lcd.print(" ");
    }
    unsigned long previousMillis = 0;
    unsigned long temps_ms; 
    const unsigned long interval = 200; // constante à 1000ms = 1s, ici 200ms

    while (erreur < 3 && victoire == 0 && perduTemps==0) {
        temps_ms = millis();
        checkChrono();
        if (button_state) {
            if(temps_ms - previousMillis >= interval) {
                // lcd.print("aaaaaaa"); // Affiche Loop sur le moniteur série toutes les constantes (10s)
                button_pressed();
            }
            previousMillis = temps_ms;
        }
    }

    //en fin de partie, si perdu(compte à rebours) ou 3 erreurs
    if (erreur == 3 || perduTemps == 1) {
        
        MasterSend=10; //10 = partie perdue
        Wire.beginTransmission(8);
        Wire.write(MasterSend);
        Wire.endTransmission();

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
    //digitalWrite(ardui_out,LOW);
    module_finished = true;
}

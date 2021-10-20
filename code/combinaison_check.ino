//cherche la présence d'un caractère dans une liste
int dans(char lettre[], char liste[]) { 
    for (int i = 0; i < 8; i++) { 
        if (liste[i] == lettre[0]) { 
            return 1; 
    }} return 0; 
}
//cherche la présence d'un chiffre dans une liste
int cdans(int num, int liste[]) {
    for (int i = 0; i < 8; i++) { 
        if (liste[i] == num) { 
            return 1; 
    }} return 0; 
}
//renvoie 1 si le cable n n'est pas branché
int pasbranche(int n) { 
    if (cdans(n,combinaison) == 1) { return 0; } else { return 1; } 
}
// ?????????
int addition() { 
    int add = 0; 
    for (int i = 0; i < 5; i++) { 
        if (combinaison[i] == 0) { add = add + i + 1; }} 
}
int libre() { 
    int lib = 0; 
    for (int i = 0; i < 5; i++) { 
        if (combinaison[i] == 0) { lib++; } 
    } return lib; 
}

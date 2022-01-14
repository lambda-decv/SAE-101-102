#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string> 
#include "config_sdl.h"

// Inclusion des headers
#include "structures.h"

using namespace std;

int lectFichier(const char nomFichier[10],int position) {
    ifstream entree(nomFichier, ios::in);
    char ligne[10];
    char cpt = 0;
    if (!entree)
        cout << "Probleme d'ouverture " << endl;
    else {
        for (int i = 0; i < position; i++) {
            entree.getline(ligne, 10);
            cpt++;
        }
        entree.close();
    }
   
    return atoi(ligne);
}

int main(int argc, char* argv[]) {
    
    return 0;
}
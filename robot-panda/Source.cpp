#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string> 
#include "config_sdl.h"

// Inclusion des headers
#include "structures.h"


using namespace std;

// Variables globales

const int TAILLE = 8;
bambou bambous[TAILLE];

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


void init_bambous(bambou tab[], int taille) {
	for (int i = 0; i < taille; i++) {
		tab[i].taille = 0;
		tab[i].rang = i + 1;
	}
}


int main(int argc, char* argv[]) {
    init_bambous(bambous, TAILLE);
    return 0;
	

}
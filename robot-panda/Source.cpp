#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include "config_sdl.h"

// Inclusion des headers
#include "structures.h"




using namespace std;


const int TAILLE = 8;
bambou bambous[TAILLE];


void init_bambous(bambou tab[], int taille) {
	for (int i = 0; i < taille; i++) {
		tab[i].taille = 0;
		tab[i].rang = i + 1;
	}
}




int main(int argc, char* argv[]) {
	init_bambous(bambous, TAILLE);

}
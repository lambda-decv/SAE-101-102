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

int taille_max(bambou tab[], int taille, int& taille_max_atteinte) {
	for (int i = 0; i < taille; i++) {
		if (tab[i].taille > taille_max_atteinte) {
			taille_max_atteinte = tab[i].taille;
		}
	}
	return taille_max_atteinte;
}

int moyenne(bambou tab[], int taille, int& moyenne) {
	int somme = 0;
	for (int i = 0; i < taille; i++) {
		somme += tab[i].taille;
	}
	moyenne = somme / 8;
	return moyenne;
}


int main(int argc, char* argv[]) {
	init_bambous(bambous, TAILLE);

}
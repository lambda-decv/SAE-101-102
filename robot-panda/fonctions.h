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

const int TAILLE = 9;
bambou bambous[TAILLE];

int lectFichier(const char nomFichier[10], int position) {
	ifstream entree(nomFichier, ios::in);
	char ligne[10];
	char cpt = 0;
	if (!entree)
		cout << "Probleme d'ouverture " << endl;
	else {
		for (int i = 0; i <= position; i++) {
			entree.getline(ligne, 10);
			cpt++;
		}
		entree.close();
	}

	return atoi(ligne);
}

void croissance(bambou tab[], int taille) {
	for (int i = 0; i < taille; i++) {
		tab[i].taille += tab[i].croissance;
	}
}


void init_bambous(bambou tab[], int taille) {
	for (int i = 0; i < taille; i++) {
		tab[i].taille = 0;
		tab[i].rang = i + 1;
		tab[i].croissance = lectFichier("config.txt", i);
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

int moy(bambou tab[], int taille, int& moyenne) {
	int somme = 0;
	for (int i = 0; i < taille; i++) {
		somme += tab[i].taille;
	}
	moyenne = somme / TAILLE;
	return moyenne;
}

void afficherStat(bambou tab[], int taille, int& moyenne, int& taille_max_atteinte) {
	cout << "Statistique : " << endl;
	cout << "Moyenne : " << moy(tab, taille, moyenne) << endl;
	cout << "Taille max :" << taille_max(tab, taille, taille_max_atteinte);
}

void affichgeBambous(bambou tab[], int taille) {
	for (int i = 0; i < TAILLE; i++) {
		cout << "Bambou n=" << tab[i].rang << endl;
		cout << "Croissance =" << tab[i].croissance << endl;
		cout << "Taille = " << tab[i].taille << endl << endl;
	}
}
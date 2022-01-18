#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string> 
#include "config_sdl.h"
#include <windows.h>

// Inclusion des headers
#include "structures.h"

const int largeur = 900;
const int hauteur = 480;
const int tailleMax = 10;
//const int croissanceForet = (5 + 4 + 2 + 1 + 4 + ... (config.txt)) / 9 soit environ 3.22;
//const int croissanceForet = 3;


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

int calculTauxCroissanceJournalier(bambou bambous[]) { //à corriger
	int tauxCroissance = 0;
	for (int i = 0; i < TAILLE; i++)
	{
		tauxCroissance = tauxCroissance + bambous[i].croissance;
	}
	tauxCroissance = (tauxCroissance / TAILLE) ;
	return tauxCroissance;
}


void init_bambous(bambou tab[], int taille) {
	for (int i = 0; i < taille; i++) {
		tab[i].taille = 0;
		tab[i].rang = i + 1;
		tab[i].croissance = lectFichier("config.txt", i);
		tab[i].pos.x = 10 + i * 30;
		tab[i].pos.y = hauteur;
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


void dessinTige(SDL_Renderer* rendu, coord coordonnees) {
	SDL_Rect bambou; //on définit le rectangle à tracer
				   //SDL_Rect est un type struct	
	bambou.x = coordonnees.x;  //coin en haut à gauche
	bambou.y = coordonnees.y;  //coin en haut à gauche
	bambou.w = 15;		//largeur
	bambou.h = 30;		//hauteur
	SDL_SetRenderDrawColor(rendu, 70, 94, 29, 255);	//pinceau vert
	SDL_RenderFillRect(rendu, &bambou); //on trace un rectangle plein

	SDL_Rect top; //on définit le rectangle à tracer
			   //SDL_Rect est un type struct	
	top.x = bambou.x - 2.5;  //coin en haut à gauche
	top.y = bambou.y - 3;  //coin en haut à gauche
	top.w = 20;		//largeur
	top.h = 4;		//hauteur
	SDL_SetRenderDrawColor(rendu, 70, 201, 29, 255);	//pinceau vert
	SDL_RenderFillRect(rendu, &top); //on trace un rectangle plein
}

void dessinBambou(SDL_Renderer* rendu, int taille, coord coordonnees) {
	for (int i = 0; i < taille; i++) {
		coordonnees.y -= 33;
		dessinTige(rendu, coordonnees);

	}
}

void dessinComplet(bambou tab[], SDL_Renderer* rendu, int taille, coord coordonnees) {

	for (int i = 0; i < taille; i++) {
		coordonnees.x += 40;
		dessinBambou(rendu, tab[i].taille, coordonnees);
	}
	SDL_RenderPresent(rendu); //sinon on ne voit rien
}

void affichageRobot(SDL_Renderer* rendu, SDL_Surface* robot, SDL_Texture* texture, coord coord) {

	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ coord.x, hauteur - 125, 75, 75 };
	SDL_QueryTexture(texture, nullptr, nullptr, &src1.w, &src1.h);
	SDL_RenderCopy(rendu, texture, &src1, &dst1); // Affiche la texture entièrement

}

coord reduceMax(bambou tab[], int taille) {
	coord coordonnees;

	int cpt = 0;
	int maxT = 0;
	int iT = 0;
	int iC = 0;
	int croissance = 0;
	int taille_max_atteinte = 0;

	maxT = tab[0].taille;
	for (int i = 1; i < taille; i++) {
		if (maxT < tab[i].taille) {
			maxT = tab[i].taille;
			iT = i;
			cpt++;
		}
	}

	croissance = tab[0].croissance;
	for (int i = 1; i < taille; i++) {
		if (croissance < tab[i].croissance) {
			croissance = tab[i].croissance;
			iC = i;
			cpt++;
		}
	}
	if (cpt > 1) {
		for (int i = 0; i < taille; i++) {
			if (tab[i].taille == maxT) {
				return tab[iC].pos;
			}
		}
	}
	return tab[iT].pos;
}


void couperBambou(bambou tab[], int taille, SDL_Renderer* rendu) {
	for (int i = 0; i < taille; i++) {
		if (reduceMax(tab, taille).x == tab[i].pos.x) {
			tab[i].taille = 1;
			dessinComplet(tab, rendu, taille, tab[i].pos);
		}
	}
	
}

void deplacerRobot(bambou tab[], int taille, SDL_Renderer* rendu, SDL_Surface* robot, SDL_Texture* texture) {
	coord co;
	co.x = largeur / 2;
	SDL_DestroyTexture(texture);
	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ co.x, hauteur - 125, 75, 75 };
	SDL_QueryTexture(texture, nullptr, nullptr, &src1.w, &src1.h);
	SDL_RenderCopy(rendu, texture, &src1, &dst1); // Affiche la texture entièrement

}

void cycleJournalier(SDL_Renderer* rendu,bambou tab[], int nbCycle, coord co) {
	for (int i = 0; i < nbCycle; i++) {
		croissance(bambous, TAILLE);
		couperBambou(bambous, TAILLE, rendu);
		dessinComplet(tab, rendu, TAILLE, co);
	}
}

/*int reduceMaxCorrection(bambou tab[], int taille) {
	int iT = 0;
	int maxT = tab[0].taille;;

	for (int i = 1; i < taille; i++) {
		if (maxT < tab[i].taille) {
			maxT = tab[i].taille;
			iT = i;
			//cpt++; pourquoi? //remise de i à 1 ?
		}
	}
	return iT;
}*/

/*int reduceFastest(bambou tab[], int taille) {
	int iFastest = 0;
	int fastestCroiss = tab[0].croissance;
	int x = 1 + 1 / sqrt(5);
	for (int i = 0; tab[i].taille > x * croissanceForet; i++)
	{
		if (fastestCroiss < tab[i].croissance) {
			fastestCroiss = tab[i].croissance;
			iFastest = i;
		}
	}
	return iFastest;
}*/
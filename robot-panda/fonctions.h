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

const int NB_JOURS = 52;


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
	moyenne = somme / float(taille);
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
}*/


void cycleJournalier(SDL_Renderer* rendu, bambou tab[], int nbCycle, coord co) {
	for (int i = 0; i < nbCycle; i++) {
		croissance(bambous, TAILLE);
		couperBambou(bambous, TAILLE, rendu);
		dessinComplet(tab, rendu, TAILLE, co);
	}
}

void graph1() {

	SDL_Point pointA;
	pointA.x = 850;
	pointA.y = 30;
	SDL_Point pointB;
	pointB.x = 850;
	pointB.y = 230;
	SDL_Point pointA2;
	pointA2.x = 850;
	pointA2.y = 230;
	SDL_Point pointB2;
	pointB2.x = 1150;
	pointB2.y = 230;

	SDL_Point pointA3;
	pointA3.x = 892;
	pointA3.y = 228;
	SDL_Point pointB3;
	pointB3.x = 892;
	pointB3.y = 232;
	SDL_Point pointA4;
	pointA4.x = 934;
	pointA4.y = 228;
	SDL_Point pointB4;
	pointB4.x = 934;
	pointB4.y = 232;
	SDL_Point pointA5;
	pointA5.x = 976;
	pointA5.y = 228;
	SDL_Point pointB5;
	pointB5.x = 976;
	pointB5.y = 232;
	SDL_Point pointA6;
	pointA6.x = 1018;
	pointA6.y = 228;
	SDL_Point pointB6;
	pointB6.x = 1018;
	pointB6.y = 232;
	SDL_Point pointA7;
	pointA7.x = 1060;
	pointA7.y = 228;
	SDL_Point pointB7;
	pointB7.x = 1060;
	pointB7.y = 232;
	SDL_Point pointA8;
	pointA8.x = 1102;
	pointA8.y = 228;
	SDL_Point pointB8;
	pointB8.x = 1102;
	pointB8.y = 232;
	SDL_Point pointA9;
	pointA9.x = 1144;
	pointA9.y = 228;
	SDL_Point pointB9;
	pointB9.x = 1144;
	pointB9.y = 232;
	SDL_Point pointA10;
	pointA10.x = 848;
	pointA10.y = 190;
	SDL_Point pointB10;
	pointB10.x = 852;
	pointB10.y = 190;
	SDL_Point pointA11;
	pointA11.x = 848;
	pointA11.y = 150;
	SDL_Point pointB11;
	pointB11.x = 852;
	pointB11.y = 150;
	SDL_Point pointA12;
	pointA12.x = 848;
	pointA12.y = 110;
	SDL_Point pointB12;
	pointB12.x = 852;
	pointB12.y = 110;
	SDL_Point pointA13;
	pointA13.x = 848;
	pointA13.y = 70;
	SDL_Point pointB13;
	pointB13.x = 852;
	pointB13.y = 70;


	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA.x, pointA.y, pointB.x, pointB.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA2.x, pointA2.y, pointB2.x, pointB2.y);

	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA3.x, pointA3.y, pointB3.x, pointB3.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA4.x, pointA4.y, pointB4.x, pointB4.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA5.x, pointA5.y, pointB5.x, pointB5.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA6.x, pointA6.y, pointB6.x, pointB6.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA7.x, pointA7.y, pointB7.x, pointB7.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA8.x, pointA8.y, pointB8.x, pointB8.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA9.x, pointA9.y, pointB9.x, pointB9.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA10.x, pointA10.y, pointB10.x, pointB10.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA11.x, pointA11.y, pointB11.x, pointB11.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA12.x, pointA12.y, pointB12.x, pointB12.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA13.x, pointA13.y, pointB13.x, pointB13.y);
}






void CourbeMoyenneTaille(SDL_Renderer* rendu, bambou tab[], coord co) {
	float somme=0.0;
	float moyennes[NB_JOURS];
	for (int i = 0; i < NB_JOURS; i++) {
		moyennes[i] = 0;
	}
	for (int i = 0; i < NB_JOURS; i++) {
		for (int j = 1; j < NB_JOURS && moyennes[j - 1] != 0; j++) {
			moyennes[j] = moyennes[j - 1];
		}
		for (int j = 0; j < TAILLE; j++) {
			somme += tab[j].taille;
		}
		moyennes[0] = somme / (float)TAILLE;
		SDL_Point point1;
		point1.x = 892;
		point1.y = 30 + (200 - moyennes[0]);
		SDL_Point point2;
		point2.x = 934;
		point2.y = 30 + (200 - moyennes[1]);
		SDL_Point point3;
		point3.x = 976;
		point3.y = 30 + (200 - moyennes[2]);
		SDL_Point point4;
		point4.x = 1018;
		point4.y = 30 + (200 - moyennes[3]);
		SDL_Point point5;
		point1.x = 1060;
		point1.y = 30 + (200 - moyennes[4]);
		SDL_Point point6;
		point6.x = 1102;
		point6.y = 30 + (200 - moyennes[5]);
		SDL_Point point7;
		point1.x = 1144;
		point1.y = 30 + (200 - moyennes[1]);

		SDL_SetRenderDrawColor(rendu, 0, 18, 252, 255);
		SDL_RenderDrawLine(rendu, point1.x, point1.y, point2.x, point2.y);
		SDL_SetRenderDrawColor(rendu, 0, 18, 252, 255);
		SDL_RenderDrawLine(rendu, point2.x, point2.y, point3.x, point3.y);
		SDL_SetRenderDrawColor(rendu, 0, 18, 252, 255);
		SDL_RenderDrawLine(rendu, point3.x, point3.y, point4.x, point4.y);
		SDL_SetRenderDrawColor(rendu, 0, 18, 252, 255);
		SDL_RenderDrawLine(rendu, point4.x, point4.y, point5.x, point5.y);
		SDL_SetRenderDrawColor(rendu, 0, 18, 252, 255);
		SDL_RenderDrawLine(rendu, point5.x, point5.y, point6.x, point6.y);
		SDL_SetRenderDrawColor(rendu, 0, 18, 252, 255);
		SDL_RenderDrawLine(rendu, point6.x, point6.y, point7.x, point7.y);
	}
}
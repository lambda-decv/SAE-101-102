#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string> 
#include "config_sdl.h"

// Inclusion des headers
#include "structures.h"

const int largeur = 900;
const int hauteur = 480;
const int tailleMax = 10;

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

void coupageBambou(bambou bambou,robot robot, int jour) {
	if (robot.pos.x == bambou.pos.x && robot.pos.y == bambou.pos.y)
		bambou.taille -= tailleMax;
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

void dessinBambou(SDL_Renderer* rendu,int taille, coord coordonnees) {
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
	int taille_max_atteinte = 0;
	for (int i = 0; i < taille; i++) {
		if (taille_max(tab,taille,taille_max_atteinte) == tab[i].taille) {
			coordonnees.x = tab[i].pos.x;
			coordonnees.y = tab[i].pos.y;
			cout << "(" << coordonnees.x << ";" << coordonnees.y << ")" << endl;
			return coordonnees;
		}
	}

}

void deplacerRobot(bambou tab[],int taille,SDL_Renderer* rendu, SDL_Surface* robot, SDL_Texture* texture) {
	coord co;
	co.x = largeur / 2;
	SDL_DestroyTexture(texture);
	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ co.x, hauteur - 125, 75, 75 };
	SDL_QueryTexture(texture, nullptr, nullptr, &src1.w, &src1.h);
	SDL_RenderCopy(rendu, texture, &src1, &dst1); // Affiche la texture entièrement

}

void couperBambou(bambou tab[], int taille, SDL_Renderer* rendu) {
	for (int i = 0; i < taille; i++) {
		if (reduceMax(tab, taille).x == tab[i].pos.x) {
			if (tab[i].taille - tailleMax < 0) {
				tab[i].taille = 0;
			}
			else {
				tab[i].taille = tab[i].taille - tailleMax;
			}
		}
	}
	for (int j = 0; j < taille; j++) {
		dessinComplet(tab, rendu, taille, tab[j].pos);
	}
}

/*coord reduceFastest(bambou tab[], int taille) {
	coord coordonnees;
	int croissance_max = 0;
	for (int i = 0; i < TAILLE; i++)
	{
		return coordonnees;
	}
}*/


void cycleJournalier(SDL_Renderer* rendu, bambou tab[], int nbCycle, coord co) {
	for (int i = 0; i < nbCycle; i++) {
		croissance(bambous, TAILLE);
		couperBambou(bambous, TAILLE, rendu);
		dessinComplet(tab, rendu, TAILLE, co);
	}
}

void CourbeMoyenneCroissance(SDL_Renderer* rendu, bambou tab[], int nbCycles, coord co) {
	float somme;
	for (int i = 0; i < nbCycles; i++) {
		somme = 0;
		for (int j = 0; j < TAILLE; j++) {
			somme += tab[j].taille;
		}
		SDL_Point pointA;
		pointA.x = (i%7)+42;
		pointA.y = 30+(200-somme/TAILLE);
	}
}


//tableau avec numéros avanceant de (i++%7)+1






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
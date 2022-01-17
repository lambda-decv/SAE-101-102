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

int calculTauxCroissance() {
	int tauxCroissance = 0;
	return 0;
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
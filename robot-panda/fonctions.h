#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string> 
#include "config_sdl.h"
#include <windows.h>
#include <ctime>

// Inclusion des headers
#include "structures.h"

const int largeur = 900;
const int hauteur = 480;
const int tailleMax = 10;


using namespace std;
// Variables globales

const int TAILLE = 4;
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
		tab[i].pos.x = 10 + i * 80;
		tab[i].pos.y = hauteur -85;
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
		cout << "Taille = " << tab[i].taille << endl;
		cout << "Coordonnees = (" << tab[i].pos.x << " , " << tab[i].pos.y << ")" << endl << endl;
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
		coordonnees.x += 80;
		dessinBambou(rendu, tab[i].taille, coordonnees);
	}
	SDL_RenderPresent(rendu); //sinon on ne voit rien
}

void affichageRobot(SDL_Renderer* rendu,coord coord,SDL_Texture* texture) {
	
	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ coord.x, hauteur - 125, 75, 75 };
	SDL_QueryTexture(texture, nullptr, nullptr, &src1.w, &src1.h);
	SDL_RenderCopy(rendu, texture, &src1, &dst1); // Affiche la texture entièrement
	SDL_RenderPresent(rendu); //sinon on ne voit rien
}

int reduceMax(bambou tab[], int taille) {
	int index = 0;
	int max = 0;
	
	max = tab[0].taille;
	for (int i = 1; i < TAILLE; i++) {
		if (tab[i].taille > max) {
			max = tab[i].taille;
			index = i;
		}
	}
	return index;
}

void couperBambou(SDL_Renderer* rendu,bambou tab[],coord co) {
	tab[reduceMax(tab, TAILLE)].taille = 1;
	//tab[rand()%8].taille = 1;
}

void deplacerRobot(bambou tab[], int taille, SDL_Renderer* rendu,SDL_Texture* texture) {

	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ tab[reduceMax(tab, TAILLE)].pos.x, tab[reduceMax(tab, TAILLE)].pos.y, 75, 75 };
	SDL_QueryTexture(texture, nullptr, nullptr, &src1.w, &src1.h);
	SDL_RenderCopy(rendu, texture, &src1, &dst1); // Affiche la texture entièrement

}
void affichageBg(SDL_Renderer* rendu,SDL_Texture* pTextureImage, SDL_Texture* pTextureImage2) {
	
	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ 0, 380, 800, 100 };
	SDL_QueryTexture(pTextureImage, nullptr, nullptr, &src1.w, &src1.h);

	SDL_Rect src2{ 0, 0, 0, 0 };
	SDL_Rect dst2{ 675, 30, 75, 75 };
	SDL_QueryTexture(pTextureImage2, nullptr, nullptr, &src2.w, &src2.h);

	SDL_Rect rectangle{ 800, 0, 400, 480 };

	SDL_SetRenderDrawColor(rendu, 0, 242, 255, 255);
	SDL_RenderClear(rendu);

	SDL_RenderCopy(rendu, pTextureImage, &src1, &dst1); // Affiche la texture entièrement
	SDL_RenderCopy(rendu, pTextureImage2, &src2, &dst2); // Affiche la texture entièrement

	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderFillRect(rendu, &rectangle);
	
	SDL_RenderPresent(rendu);
}

void cycleJournalier(SDL_Renderer* rendu, bambou tab[], int nbCycle, coord co, SDL_Texture* pTextureImage, SDL_Texture* pTextureImage2, SDL_Texture* pTextureRobot) {
	init_bambous(tab, TAILLE);
	for (int i = 0; i < nbCycle; i++) {
		croissance(tab, TAILLE);
		couperBambou(rendu,tab,co);
		SDL_RenderClear(rendu);
		affichageBg(rendu,pTextureImage,pTextureImage2);
		affichageRobot(rendu, tab[reduceMax(tab, TAILLE)].pos, pTextureRobot);
		dessinComplet(tab, rendu, TAILLE, co);
		Sleep(2000);
	}

}


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

const int largeur = 1200;
const int hauteur = 575;
const int tailleMax = 10;

const int NB_JOURS = 7;

int redFast, redMax, algo;


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


int moy(bambou tab[]) {
	float moyenne;
	int somme = 0;
	for (int i = 0; i < TAILLE; i++) {
		somme += tab[i].taille;
	}
	moyenne = somme / float(TAILLE);
	return moyenne;
}

void init_bambous(bambou tab[], int taille) {
	for (int i = 0; i < taille; i++) {
		tab[i].croissance = lectFichier("config.txt", i);
		tab[i].taille = tab[i].croissance;
		tab[i].rang = i + 1;
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

void afficherStat(bambou tab[], int taille, int& moyenne, int& taille_max_atteinte) {
	cout << "Statistique : " << endl;
	cout << "Moyenne : " << moy(tab) << endl;
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
	bambou.y = coordonnees.y - 100;  //coin en haut à gauche
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
	SDL_Rect dst1{ coord.x, hauteur - 200, 75, 75 };
	SDL_QueryTexture(texture, nullptr, nullptr, &src1.w, &src1.h);
	SDL_RenderCopy(rendu, texture, &src1, &dst1); // Affiche la texture entièrement
	SDL_RenderPresent(rendu); //sinon on ne voit rien
}

int reduceMax(bambou tab[]) {
	int index = 0;
	float H = 19 / 5;
	
	int croissance_max = tab[0].croissance;
	int max = tab[0].taille;
	for (int i = 1; i < TAILLE; i++) {
		if (tab[i].taille > max) {
			max = tab[i].taille;
			index = i;
		}
		if (tab[i].taille==max) {
			if (tab[i].croissance > croissance_max) {
				croissance_max = tab[i].croissance;
				index = i;
			}
		}
	}
	cout << "Index = " << index << endl;
	return index;
}

void CourbeMoyenneTaille(SDL_Renderer* rendu, bambou tab[]) {
	float somme = 0.0;
	float moyennes[NB_JOURS];
	for (int i = 0; i < NB_JOURS; i++) {
		moyennes[i] = 0;
	}
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
	point5.x = 1060;
	point5.y = 30 + (200 - moyennes[4]);
	SDL_Point point6;
	point6.x = 1102;
	point6.y = 30 + (200 - moyennes[5]);
	SDL_Point point7;
	point7.x = 1144;
	point7.y = 30 + (200 - moyennes[6]);

	if (moyennes[1] != 0) {
		SDL_SetRenderDrawColor(rendu, 0, 18, 252, 255);
		SDL_RenderDrawLine(rendu, point1.x, point1.y, point2.x, point2.y);
	}
	if (moyennes[2] != 0) {
		SDL_SetRenderDrawColor(rendu, 0, 18, 252, 255);
		SDL_RenderDrawLine(rendu, point2.x, point2.y, point3.x, point3.y);
	}
	if (moyennes[3] != 0) {
		SDL_SetRenderDrawColor(rendu, 0, 18, 252, 255);
		SDL_RenderDrawLine(rendu, point3.x, point3.y, point4.x, point4.y);
	}
	if (moyennes[4] != 0) {
		SDL_SetRenderDrawColor(rendu, 0, 18, 252, 255);
		SDL_RenderDrawLine(rendu, point4.x, point4.y, point5.x, point5.y);
	}
	if (moyennes[5] != 0) {
		SDL_SetRenderDrawColor(rendu, 0, 18, 252, 255);
		SDL_RenderDrawLine(rendu, point5.x, point5.y, point6.x, point6.y);
	}
	if (moyennes[6] != 0) {
		SDL_SetRenderDrawColor(rendu, 0, 18, 252, 255);
		SDL_RenderDrawLine(rendu, point6.x, point6.y, point7.x, point7.y);
	}
}

void couperBambou(bambou tab[], int index) {
	tab[reduceMax(tab)].taille = 1;
	//tab[rand()%8].taille = 1;
	
}

void deplacerRobot(bambou tab[], int taille, SDL_Renderer* rendu,SDL_Texture* texture) {

	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ tab[reduceMax(tab)].pos.x, tab[reduceMax(tab)].pos.y, 75, 75 };
	SDL_QueryTexture(texture, nullptr, nullptr, &src1.w, &src1.h);
	SDL_RenderCopy(rendu, texture, &src1, &dst1); // Affiche la texture entièrement

}

void boutonChangeMod(SDL_Renderer* rendu) {
	SDL_Rect changeMod;
	changeMod.x = 32;
	changeMod.y = 488;
	changeMod.w = 200;
	changeMod.h = 35;

	SDL_SetRenderDrawColor(rendu, 0, 0, 255, 255);
	SDL_RenderDrawRect(rendu, &changeMod);
	SDL_RenderFillRect(rendu, &changeMod);
}

void enterXFastest(SDL_Renderer* rendu) {
	SDL_Rect enterX;
	enterX.x = 32;
	enterX.y = 532;
	enterX.w = 200;
	enterX.h = 35;

	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawRect(rendu, &enterX);
}

void boutonPlay(SDL_Renderer* rendu) {
	SDL_Rect play;
	play.x = 1168;
	play.y = 488;
	play.w = -200;
	play.h = 35;

	SDL_SetRenderDrawColor(rendu, 0, 255, 0, 255);
	SDL_RenderDrawRect(rendu, &play);
	SDL_RenderFillRect(rendu, &play);
}

void boutonPause(SDL_Renderer* rendu) {
	SDL_Rect pause;
	pause.x = 1168;
	pause.y = 532;
	pause.w = -200;
	pause.h = 35;

	SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
	SDL_RenderDrawRect(rendu, &pause);
	SDL_RenderFillRect(rendu, &pause);
}

void rectBouton(SDL_Renderer* rendu) {
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 480;
	rect.w = 1200;
	rect.h = 95;

	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderDrawRect(rendu, &rect);
	SDL_RenderFillRect(rendu, &rect);

	boutonChangeMod(rendu);
	enterXFastest(rendu);
	boutonPlay(rendu);
	boutonPause(rendu);

	SDL_RenderPresent(rendu);
}

void boutonsdirection(SDL_Renderer* rendu, SDL_Texture* pTextureBoutonD) {
	SDL_Rect src_boutond{ 0, 0, 0, 0 };
	SDL_Rect dst_boutond{ 800, 490, 75, 75 };
	SDL_QueryTexture(pTextureBoutonD, nullptr, nullptr, &src_boutond.w, &src_boutond.h);
	SDL_RenderCopy(rendu, pTextureBoutonD, &src_boutond, &dst_boutond); // Affiche la texture entièrement
}

void affichageBg(SDL_Renderer* rendu,SDL_Texture* pTextureImage, SDL_Texture* pTextureImage2, SDL_Texture* pTextureBoutonD) {
	
	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ 0, 380, 800, 100 };
	SDL_QueryTexture(pTextureImage, nullptr, nullptr, &src1.w, &src1.h);

	SDL_Rect src2{ 0, 0, 0, 0 };
	SDL_Rect dst2{ 675, 30, 75, 75 };
	SDL_QueryTexture(pTextureImage2, nullptr, nullptr, &src2.w, &src2.h);

	SDL_Rect rectangle{ 800, 0, 400, 480 };

	SDL_Rect src_boutond{ 0, 0, 0, 0 };
	SDL_Rect dst_boutond{ 725, 490, 75, 75 };

	SDL_SetRenderDrawColor(rendu, 0, 242, 255, 255);
	SDL_RenderClear(rendu);

	SDL_RenderCopy(rendu, pTextureImage, &src1, &dst1); // Affiche la texture entièrement
	SDL_RenderCopy(rendu, pTextureImage2, &src2, &dst2); // Affiche la texture entièrement

	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderFillRect(rendu, &rectangle);

	rectBouton(rendu);
	
	SDL_RenderCopy(rendu, pTextureBoutonD, &src_boutond, &dst_boutond); // Affiche la texture entièrement
	
	SDL_RenderPresent(rendu);
}


int reduceFastest(bambou tab[]) {
	int x = 1.45;
	int index = 0;
	int coeffMax = 0;
	int limitePousse = x * hauteur - 95;

	for (int i = 0; i < TAILLE; i++) {
		if (limitePousse < tab[i].taille) {
			if (coeffMax < tab[i].croissance) {
				coeffMax = tab[i].croissance;
				index = i;
			}
		}
	}
	return index;
}

void graph1(SDL_Renderer* r) {

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


	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA.x, pointA.y, pointB.x, pointB.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA2.x, pointA2.y, pointB2.x, pointB2.y);

	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA3.x, pointA3.y, pointB3.x, pointB3.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA4.x, pointA4.y, pointB4.x, pointB4.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA5.x, pointA5.y, pointB5.x, pointB5.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA6.x, pointA6.y, pointB6.x, pointB6.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA7.x, pointA7.y, pointB7.x, pointB7.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA8.x, pointA8.y, pointB8.x, pointB8.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA9.x, pointA9.y, pointB9.x, pointB9.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA10.x, pointA10.y, pointB10.x, pointB10.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA11.x, pointA11.y, pointB11.x, pointB11.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA12.x, pointA12.y, pointB12.x, pointB12.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA13.x, pointA13.y, pointB13.x, pointB13.y);
}

void graph2(SDL_Renderer* r) {

	SDL_Point pointA;
	pointA.x = 850;
	pointA.y = 270;
	SDL_Point pointB;
	pointB.x = 850;
	pointB.y = 470;
	SDL_Point pointA2;
	pointA2.x = 850;
	pointA2.y = 470;
	SDL_Point pointB2;
	pointB2.x = 1150;
	pointB2.y = 470;

	SDL_Point pointA3;
	pointA3.x = 892;
	pointA3.y = 468;
	SDL_Point pointB3;
	pointB3.x = 892;
	pointB3.y = 472;
	SDL_Point pointA4;
	pointA4.x = 934;
	pointA4.y = 468;
	SDL_Point pointB4;
	pointB4.x = 934;
	pointB4.y = 472;
	SDL_Point pointA5;
	pointA5.x = 976;
	pointA5.y = 468;
	SDL_Point pointB5;
	pointB5.x = 976;
	pointB5.y = 472;
	SDL_Point pointA6;
	pointA6.x = 1018;
	pointA6.y = 468;
	SDL_Point pointB6;
	pointB6.x = 1018;
	pointB6.y = 472;
	SDL_Point pointA7;
	pointA7.x = 1060;
	pointA7.y = 468;
	SDL_Point pointB7;
	pointB7.x = 1060;
	pointB7.y = 472;
	SDL_Point pointA8;
	pointA8.x = 1102;
	pointA8.y = 468;
	SDL_Point pointB8;
	pointB8.x = 1102;
	pointB8.y = 472;
	SDL_Point pointA9;
	pointA9.x = 1144;
	pointA9.y = 468;
	SDL_Point pointB9;
	pointB9.x = 1144;
	pointB9.y = 472;
	SDL_Point pointA10;
	pointA10.x = 848;
	pointA10.y = 430;
	SDL_Point pointB10;
	pointB10.x = 852;
	pointB10.y = 430;
	SDL_Point pointA11;
	pointA11.x = 848;
	pointA11.y = 390;
	SDL_Point pointB11;
	pointB11.x = 852;
	pointB11.y = 390;
	SDL_Point pointA12;
	pointA12.x = 848;
	pointA12.y = 350;
	SDL_Point pointB12;
	pointB12.x = 852;
	pointB12.y = 350;
	SDL_Point pointA13;
	pointA13.x = 848;
	pointA13.y = 310;
	SDL_Point pointB13;
	pointB13.x = 852;
	pointB13.y = 310;


	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA.x, pointA.y, pointB.x, pointB.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA2.x, pointA2.y, pointB2.x, pointB2.y);

	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA3.x, pointA3.y, pointB3.x, pointB3.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA4.x, pointA4.y, pointB4.x, pointB4.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA5.x, pointA5.y, pointB5.x, pointB5.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA6.x, pointA6.y, pointB6.x, pointB6.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA7.x, pointA7.y, pointB7.x, pointB7.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA8.x, pointA8.y, pointB8.x, pointB8.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA9.x, pointA9.y, pointB9.x, pointB9.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA10.x, pointA10.y, pointB10.x, pointB10.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA11.x, pointA11.y, pointB11.x, pointB11.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA12.x, pointA12.y, pointB12.x, pointB12.y);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderDrawLine(r, pointA13.x, pointA13.y, pointB13.x, pointB13.y);


}

void cycleJournalier(SDL_Renderer* rendu, bambou tab[],coord co, SDL_Texture* pTextureImage, SDL_Texture* pTextureImage2, SDL_Texture* pTextureRobot,SDL_Texture* pTextureBoutonD, int algo) {
	int index = 0;
	if (algo == 1) {
		index = reduceMax(tab);
	}
	else if (algo == 2) {
		index = reduceFastest(tab);
	}
	croissance(tab, TAILLE);
	couperBambou(tab,index);
	SDL_RenderClear(rendu);
	affichageBg(rendu,pTextureImage,pTextureImage2,pTextureBoutonD);
	graph1(rendu);
	graph2(rendu);
	affichageRobot(rendu, tab[reduceMax(tab)].pos, pTextureRobot);
	rectBouton(rendu);
	dessinComplet(tab, rendu, TAILLE, co);
}

void menu(SDL_Renderer* rendu, TTF_Font* font) {
	SDL_RenderClear(rendu);
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderClear(rendu);
	
	SDL_Color noir = { 0,0,0 }; 
	SDL_Rect positionTexte; 
	positionTexte.x = largeur/2 +50;
	positionTexte.y = 35;

	SDL_Texture* texture = loadText(rendu, "MENU", noir, font);

	SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);

	positionTexte.w *= 3;
	positionTexte.h *= 3;

	SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
	//on détruit la texture
	SDL_DestroyTexture(texture);
	
	SDL_Point pointA;
	pointA.x = positionTexte.x + 100;
	pointA.y = positionTexte.y + 100;
	SDL_Point pointB;
	pointB.x = positionTexte.x + 100;
	pointB.y = hauteur - 10;


	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA.x, pointA.y, pointB.x, pointB.y);

	//on met à jour le rendu - FONCTION CAPITALE
	SDL_RenderPresent(rendu); //sinon on ne voit rien

}
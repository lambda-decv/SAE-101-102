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

#define ORIGINE_GRAPH_1_X 850
#define ORIGINE_GRAPH_1_Y 230

#define ORIGINE_GRAPH_2_X 850
#define ORIGINE_GRAPH_2_Y 470

#define ROBOT_START_X 725
#define ROBOT_START_Y 405

coord pandaStart = { ROBOT_START_X , ROBOT_START_Y };

const int largeur = 1200;
const int hauteur = 575;
const int tailleMax = 10;

const int NB_JOURS = 7;

int redFast, redMax, algo;
const int nbPt = 10;
int cpt = 0;
coord tabCo[nbPt];
using namespace std;
// Variables globales
int x = 850;
int y = 230;
int y2 = 470;
SDL_Point ptPrecedent{x,y};
SDL_Point ptPrecedentMax{x,y2};

int niveauBattery;

const int TAILLE = 5;
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

float moy(bambou tab[]) {
	float moyenne;
	int somme = 0;
	for (int i = 0; i < TAILLE; i++) {
		somme += tab[i].taille;
	}
	moyenne = float(somme) / float(TAILLE);
	return  moyenne;
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

int taille_max(bambou tab[]) {
	int taille_max = 0;
	for (int i = 0; i < TAILLE; i++) {
		if (tab[i].taille > taille_max) {
			taille_max = tab[i].taille;
		}
	}
	return taille_max;
}

void afficherStat(bambou tab[], int taille, int& moyenne, int& taille_max_atteinte) {
	cout << "Statistique : " << endl;
	cout << "Moyenne : " << moy(tab) << endl;
	cout << "Taille max :" << taille_max(tab);
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

		coordonnees.x += 50;
		dessinBambou(rendu, tab[i].taille, coordonnees);
	}
	SDL_RenderPresent(rendu); //sinon on ne voit rien
}

void affichageRobot(SDL_Renderer* rendu,coord coord,SDL_Texture* texture, int& indice_panda) {
	
	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ coord.x, hauteur - 200, 75, 75 };
	if (coord.x < 800 && coord.x>0) {
		SDL_QueryTexture(texture, nullptr, nullptr, &src1.w, &src1.h);
		SDL_RenderCopy(rendu, texture, &src1, &dst1); // Affiche la texture entièrement
		SDL_RenderPresent(rendu); //sinon on ne voit rien
	}
	else {
		indice_panda = TAILLE - 1;
		SDL_Rect src1{ 0, 0, 0, 0 };
		SDL_Rect dst1{ indice_panda, hauteur - 200, 75, 75 };
		SDL_QueryTexture(texture, nullptr, nullptr, &src1.w, &src1.h);
		SDL_RenderCopy(rendu, texture, &src1, &dst1); // Affiche la texture entièrement
		SDL_RenderPresent(rendu); //sinon on ne voit rien
	}
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
	return index;
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

void couperBambou(bambou tab[], int index) {
	tab[index].taille = 1;	
	//tab[rand()%8].taille = 1;
}

void deplacerRobot(bambou tab[], int taille, SDL_Renderer* rendu,SDL_Texture* texture, int& indice_panda) {

	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ tab[indice_panda].pos.x, tab[indice_panda].pos.y, 75, 75 };
	SDL_QueryTexture(texture, nullptr, nullptr, &src1.w, &src1.h);
	SDL_RenderCopy(rendu, texture, &src1, &dst1); // Affiche la texture entièrement

}

void deplaceravecboutonD(int& indice_panda, bambou tab[], SDL_Renderer* rendu, SDL_Texture* texture) {
	affichageRobot(rendu, tab[indice_panda + 1].pos, texture, indice_panda);
	indice_panda += 1;
}

void deplaceravecboutonG(int& indice_panda, bambou tab[], SDL_Renderer* rendu, SDL_Texture* texture) {
	affichageRobot(rendu, tab[indice_panda - 1].pos, texture, indice_panda);
	indice_panda -= 1;
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
	play.x = 968;
	play.y = 488;
	play.w = 200;
	play.h = 35;

	SDL_SetRenderDrawColor(rendu, 0, 255, 0, 255);
	SDL_RenderDrawRect(rendu, &play);
	SDL_RenderFillRect(rendu, &play);
}

void boutonPause(SDL_Renderer* rendu) {
	SDL_Rect pause;
	pause.x = 968;
	pause.y = 532;
	pause.w = 200;
	pause.h = 35;

	SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
	SDL_RenderDrawRect(rendu, &pause);
	SDL_RenderFillRect(rendu, &pause);
}

void boutonsdirection(SDL_Renderer* rendu, SDL_Texture* pTextureBoutonD, SDL_Texture* pTextureBoutonG) {
	SDL_Rect src_boutond{ 0, 0, 0, 0 };
	SDL_Rect dst_boutond{ 725, 505, 50, 50 };
	SDL_QueryTexture(pTextureBoutonD, nullptr, nullptr, &src_boutond.w, &src_boutond.h);
	SDL_RenderCopy(rendu, pTextureBoutonD, &src_boutond, &dst_boutond); // Affiche la texture entièrement
	SDL_Rect src_boutong{ 0, 0, 0, 0 };
	SDL_Rect dst_boutong{ 650, 505, 50, 50 };
	SDL_QueryTexture(pTextureBoutonG, nullptr, nullptr, &src_boutong.w, &src_boutong.h);
	SDL_RenderCopy(rendu, pTextureBoutonG, &src_boutong, &dst_boutong); // Affiche la texture entièrement
}

void boutoncouper(SDL_Renderer* rendu, SDL_Texture* pTextureBoutonC) {
	SDL_Rect src_boutonc{ 0, 0, 0, 0 };
	SDL_Rect dst_boutonc{ 575, 505, 50, 50 };
	SDL_QueryTexture(pTextureBoutonC, nullptr, nullptr, &src_boutonc.w, &src_boutonc.h);
	SDL_RenderCopy(rendu, pTextureBoutonC, &src_boutonc, &dst_boutonc); // Affiche la texture entièrement
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

void affichageTxtPlay(SDL_Renderer* rendu, TTF_Font* font) {
	SDL_Color noir = { 0,0,0 };
	SDL_Rect positionTexte;

	positionTexte.x = 1045;
	positionTexte.y = 492;
	
	SDL_Texture* texture = loadText(rendu, "Play", noir, font);

	SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);

	positionTexte.w *= 1.2;
	positionTexte.h *= 1.2;
	
	SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
	SDL_DestroyTexture(texture);
	SDL_RenderPresent(rendu);
}

void affichageTxtPause(SDL_Renderer* rendu, TTF_Font* font) {
	SDL_Color noir = { 0,0,0 };
	SDL_Rect positionTexte;

	positionTexte.x = 1035;
	positionTexte.y = 536;

	SDL_Texture* texture = loadText(rendu, "Pause", noir, font);

	SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);

	positionTexte.w *= 1.2;
	positionTexte.h *= 1.2;

	SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
	SDL_DestroyTexture(texture);
	SDL_RenderPresent(rendu);
}

void affichageTxtChangeMod(SDL_Renderer* rendu, TTF_Font* font) {
	SDL_Color noir = { 0,0,0 };
	SDL_Rect positionTexte;

	positionTexte.x = 58;
	positionTexte.y = 492;

	SDL_Texture* texture = loadText(rendu, "ChangeMod", noir, font);

	SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);

	positionTexte.w *= 1.2;
	positionTexte.h *= 1.2;

	SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
	SDL_DestroyTexture(texture);
	SDL_RenderPresent(rendu);
}

void affichageTxtValueX(SDL_Renderer* rendu, TTF_Font* font) {
	SDL_Color noir = { 0,0,0 };
	SDL_Rect positionTexte;

	positionTexte.x = 90;
	positionTexte.y = 536;

	SDL_Texture* texture = loadText(rendu, "X = ", noir, font);

	SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);

	positionTexte.w *= 1.2;
	positionTexte.h *= 1.2;

	SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
	SDL_DestroyTexture(texture);
	SDL_RenderPresent(rendu);
}

void affichageBg(SDL_Renderer* rendu,SDL_Texture* pTextureImage, SDL_Texture* pTextureImage2, SDL_Texture* pTextureBoutonD, SDL_Texture* pTextureBoutonG, SDL_Texture* pTextureBoutonC) {
	
	SDL_Rect fond{ 0, 0, 800, 480 };
	SDL_SetRenderDrawColor(rendu, 0, 242, 255, 255);	
	SDL_RenderFillRect(rendu, &fond);
	SDL_RenderPresent(rendu);

	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ 0, 380, 800, 100 };
	SDL_QueryTexture(pTextureImage, nullptr, nullptr, &src1.w, &src1.h);

	SDL_Rect src2{ 0, 0, 0, 0 };
	SDL_Rect dst2{ 675, 30, 75, 75 };
	SDL_QueryTexture(pTextureImage2, nullptr, nullptr, &src2.w, &src2.h);

	SDL_RenderCopy(rendu, pTextureImage, &src1, &dst1); // Affiche la texture entièrement
	SDL_RenderCopy(rendu, pTextureImage2, &src2, &dst2); // Affiche la texture entièrement
	rectBouton(rendu);
	
	SDL_RenderPresent(rendu);
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


	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
	SDL_RenderDrawLine(r, pointA.x, pointA.y, pointB.x, pointB.y);
	SDL_RenderDrawLine(r, pointA2.x, pointA2.y, pointB2.x, pointB2.y);
	SDL_RenderDrawLine(r, pointA3.x, pointA3.y, pointB3.x, pointB3.y);
	SDL_RenderDrawLine(r, pointA4.x, pointA4.y, pointB4.x, pointB4.y);
	SDL_RenderDrawLine(r, pointA5.x, pointA5.y, pointB5.x, pointB5.y);
	SDL_RenderDrawLine(r, pointA6.x, pointA6.y, pointB6.x, pointB6.y);
	SDL_RenderDrawLine(r, pointA7.x, pointA7.y, pointB7.x, pointB7.y);
	SDL_RenderDrawLine(r, pointA8.x, pointA8.y, pointB8.x, pointB8.y);
	SDL_RenderDrawLine(r, pointA9.x, pointA9.y, pointB9.x, pointB9.y);
	SDL_RenderDrawLine(r, pointA10.x, pointA10.y, pointB10.x, pointB10.y);
	SDL_RenderDrawLine(r, pointA11.x, pointA11.y, pointB11.x, pointB11.y);
	SDL_RenderDrawLine(r, pointA12.x, pointA12.y, pointB12.x, pointB12.y);
	SDL_RenderDrawLine(r, pointA13.x, pointA13.y, pointB13.x, pointB13.y);
	SDL_RenderPresent(r); //sinon on ne voit rien

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

void courbeMoyenneTaille(bambou tab[],SDL_Renderer* rendu, int cpt) {

	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, ptPrecedent.x, ptPrecedent.y, ORIGINE_GRAPH_1_X + cpt % 10 *30, ORIGINE_GRAPH_1_Y - moy(tab)*10);
	ptPrecedent.x = ORIGINE_GRAPH_1_X + cpt * 30;
	ptPrecedent.y = ORIGINE_GRAPH_1_Y - moy(tab) * 10;
	SDL_RenderPresent(rendu); //sinon on ne voit rien

	if (cpt == nbPt-1) {
		ptPrecedent.x = ORIGINE_GRAPH_1_X;
		ptPrecedent.y = ORIGINE_GRAPH_1_Y - moy(tab) * 10;
	}

}

void courbeMaxTaille(bambou tab[], SDL_Renderer* rendu, int cpt) {

	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, ptPrecedentMax.x, ptPrecedentMax.y, ORIGINE_GRAPH_2_X + cpt % 10 * 30, ORIGINE_GRAPH_2_Y - taille_max(tab) * 10);
	ptPrecedentMax.x = ORIGINE_GRAPH_2_X + cpt % 10 * 30;
	ptPrecedentMax.y = ORIGINE_GRAPH_2_Y - taille_max(tab) * 10;
	SDL_RenderPresent(rendu); //sinon on ne voit rien


	if (cpt == nbPt - 1) {
		ptPrecedentMax.x = ORIGINE_GRAPH_2_X;
		ptPrecedentMax.y = ORIGINE_GRAPH_2_Y - taille_max(tab) * 10;
	}
}

void battery(SDL_Renderer* rendu,int niveauBattery) {
	SDL_Rect battery; 
	battery.x = 805;
	battery.y = 440;
	battery.w = 20;
	battery.h = 40;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawRect(rendu, &battery);
	SDL_Rect batteryTop;
	batteryTop.x = battery.x + 2;
	batteryTop.y = battery.y - 5;
	batteryTop.w = 16;
	batteryTop.h = 5;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawRect(rendu, &batteryTop);

	SDL_Rect charge;
	charge.x = battery.x + 5;
	charge.y = battery.y + battery.h;
	charge.w = battery.w / 2;
	charge.h = battery.h / 11;

	for (int i = 0; i< niveauBattery; i++) {
		charge.y -= charge.h + 2;
		SDL_SetRenderDrawColor(rendu, 0, 227, 11, 255);
		SDL_RenderFillRect(rendu, &charge);
	}
	SDL_RenderPresent(rendu);
}

void cleanBambou(SDL_Renderer* rendu) {
	SDL_Rect rectangle{ 0, 0, 800, 480 };
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderFillRect(rendu, &rectangle);
	SDL_RenderPresent(rendu);
}

void cleanCourbe(SDL_Renderer* rendu) {
	SDL_Rect fond{ 850, 0, 400, 480 };
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderFillRect(rendu, &fond);
	graph1(rendu);
	graph2(rendu);
	SDL_RenderPresent(rendu);

}

void cycleJournalier(SDL_Renderer* rendu, bambou tab[],coord co, SDL_Texture* pTextureImage, SDL_Texture* pTextureImage2, SDL_Texture* pTextureRobot,SDL_Texture* pTextureBoutonD, int algo, SDL_Texture* pTextureBoutonG, SDL_Texture* pTextureBoutonC) {
	int indice_panda = TAILLE - 1;
	niveauBattery = 7 - cpt;
	if (niveauBattery < 0) {
		affichageRobot(rendu, pandaStart, pTextureRobot, indice_panda);
		cpt++;
		niveauBattery = 7;
	}
	int index = 0;
	cout << niveauBattery << endl;
	if (algo == 1) {
		index = reduceMax(tab);
	}
	else if (algo == 2) {
		index = reduceFastest(tab);
	}
	graph1(rendu);
	graph2(rendu);
	croissance(tab, TAILLE);
	couperBambou(tab, index);
	courbeMoyenneTaille(tab, rendu, cpt);
	courbeMaxTaille(tab, rendu, cpt);
	cleanBambou(rendu);
	affichageBg(rendu,pTextureImage,pTextureImage2,pTextureBoutonD,pTextureBoutonG,pTextureBoutonC);
	rectBouton(rendu);
	boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
	boutoncouper(rendu, pTextureBoutonC);
	dessinComplet(tab, rendu, TAILLE, co);
	affichageRobot(rendu, tab[reduceMax(tab)].pos, pTextureRobot, indice_panda);
	affichageTxtPlay(rendu, TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25));
	affichageTxtPause(rendu, TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25));
	affichageTxtChangeMod(rendu, TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25));
	affichageTxtValueX(rendu, TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25));
	if (cpt >= 9) {
		cpt = 0;
		cleanCourbe(rendu);
	}
	battery(rendu, niveauBattery);

	cpt++;
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
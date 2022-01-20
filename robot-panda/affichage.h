#include "includes.h"
#include "structures.h"
extern const int LARGEUR;
extern const int HAUTEUR;
extern const int TAILLE;
void dessinTige(SDL_Renderer* rendu, coord coordonnees) {
	SDL_Rect bambou; //on définit le rectangle à tracer
				   //SDL_Rect est un type struct	
	bambou.x = coordonnees.x;  //coin en haut à gauche
	bambou.y = coordonnees.y - 100;  //coin en haut à gauche
	bambou.w = 15;		//LARGEUR
	bambou.h = 30;		//HAUTEUR
	SDL_SetRenderDrawColor(rendu, 70, 94, 29, 255);	//pinceau vert
	SDL_RenderFillRect(rendu, &bambou); //on trace un rectangle plein

	SDL_Rect top; //on définit le rectangle à tracer
			   //SDL_Rect est un type struct	
	top.x = bambou.x - 2.5;  //coin en haut à gauche
	top.y = bambou.y - 3;  //coin en haut à gauche
	top.w = 20;		//LARGEUR
	top.h = 4;		//HAUTEUR
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

void affichageRobot(SDL_Renderer* rendu, coord coord, SDL_Texture* texture, int& indice_panda) {
	cout << coord.x << " " << coord.y << endl;
	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ coord.x, HAUTEUR - 200, 75, 75 };
	if (coord.x < 800 && coord.x>0) {
		SDL_QueryTexture(texture, nullptr, nullptr, &src1.w, &src1.h);
		SDL_RenderCopy(rendu, texture, &src1, &dst1); // Affiche la texture entièrement
		SDL_RenderPresent(rendu); //sinon on ne voit rien
	}
	else {
		indice_panda = TAILLE - 1;
		SDL_Rect src1{ 0, 0, 0, 0 };
		SDL_Rect dst1{ indice_panda, HAUTEUR - 200, 75, 75 };
		SDL_QueryTexture(texture, nullptr, nullptr, &src1.w, &src1.h);
		SDL_RenderCopy(rendu, texture, &src1, &dst1); // Affiche la texture entièrement
		SDL_RenderPresent(rendu); //sinon on ne voit rien
	}
}
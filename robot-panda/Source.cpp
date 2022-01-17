#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string> 
#include "config_sdl.h"

// Inclusion des headers
#include "fonctions.h"


using namespace std;

const int LARGEUR = 800;
const int HAUTEUR = 480;



int main(int argc, char* argv[]) {
	coord coordonnees;
	coordonnees.x = 5;
	coordonnees.y = HAUTEUR;
	int moyenne = 0;
	int taille_max_atteinte = 0;
	int jour = 1;
    init_bambous(bambous, TAILLE);
	cycleJour(jour);
	affichgeBambous(bambous, TAILLE);
	taille_max(bambous, TAILLE, taille_max_atteinte);
	moy(bambous, TAILLE, moyenne);
	afficherStat(bambous, TAILLE, moyenne, taille_max_atteinte);


	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Echec à louverture";
		return 1;
	}

	//on crée la fenêtre
	SDL_Window* win = SDL_CreateWindow("Ma fenetre",
		SDL_WINDOWPOS_CENTERED,     //pos. X: autre option: SDL_WINDOWPOS_UNDEFINED
		SDL_WINDOWPOS_CENTERED,     //pos. Y: autre option: SDL_WINDOWPOS_UNDEFINED 
		LARGEUR, 			//largeur en pixels			
		HAUTEUR, 			//hauteur en pixels
		SDL_WINDOW_SHOWN //dautres options (plein ecran, resizable, sans bordure...)
	);
	if (win == NULL)
		cout << "erreur ouverture fenetre";

	//Création dun dessin associé à la fenêtre (1 seul renderer par fenetre)
	SDL_Renderer* rendu = SDL_CreateRenderer(
		win,  //nom de la fenêtre
		-1, //par défaut
		SDL_RENDERER_ACCELERATED); //utilisation du GPU, valeur recommandée

	dessinComplet(bambous, rendu, TAILLE, coordonnees);
	SDL_RenderPresent(rendu);

	bool continuer = true;   //booléen fin de programme
	SDL_Event event;//gestion des évènements souris/clavier, 
					//SDL_Event est de type struct

	SDL_Surface* image = IMG_Load("sol.png");
	SDL_Texture* pTextureImage = SDL_CreateTextureFromSurface(rendu, image);

	SDL_FreeSurface(image);
	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ 0, 380, 800, 100 };

	SDL_QueryTexture(pTextureImage, nullptr, nullptr, &src1.w, &src1.h);

	while (continuer)
	{
		SDL_WaitEvent(&event);//attente dun évènement
		switch (event.type) //test du type dévènement
		{
		case SDL_QUIT: //clic sur la croix de fermeture
					   //on peut enlever SDL_Delay
			continuer = false;
			break;
		}
		SDL_SetRenderDrawColor(rendu, 0, 242, 255, 255);
		SDL_RenderClear(rendu);

		SDL_RenderCopy(rendu, pTextureImage, &src1, &dst1); // Affiche la texture entièrement
		SDL_RenderPresent(rendu);
	}

	
	SDL_DestroyTexture(pTextureImage);
	//destruction du renderer à la fin
	SDL_DestroyRenderer(rendu);

	//destruction à la fin
	SDL_DestroyWindow(win);   //equivalent du delete

	//fermeture
	SDL_Quit();
	return 0;
}
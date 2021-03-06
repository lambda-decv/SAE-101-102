#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string> 
#include "config_sdl.h"

// Inclusion des headers
#include "structures.h"
#include "fonctions.h"


using namespace std;

// Variables globales

const int TAILLE = 8;
bambou bambous[TAILLE];

const int LARGEUR = 1280; //largeur fenetre
const int HAUTEUR = 720;  //hauteur fenetre

using namespace std;



int interface() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Echec ? l?ouverture";
		return 1;
	}

	//on cr?e la fen?tre
	SDL_Window* win = SDL_CreateWindow("Ma fenetre",
		SDL_WINDOWPOS_CENTERED,     //pos. X: autre option: SDL_WINDOWPOS_UNDEFINED
		SDL_WINDOWPOS_CENTERED,     //pos. Y: autre option: SDL_WINDOWPOS_UNDEFINED 
		LARGEUR, 			//largeur en pixels			
		HAUTEUR, 			//hauteur en pixels
		SDL_WINDOW_SHOWN //d?autres options (plein ecran, resizable, sans bordure...)
	);
	if (win == NULL)
		cout << "erreur ouverture fenetre";

	//Cr?ation d?un dessin associ? ? la fen?tre (1 seul renderer par fenetre)
	SDL_Renderer* rendu = SDL_CreateRenderer(
		win,  //nom de la fen?tre
		-1, //par d?faut
		SDL_RENDERER_ACCELERATED); //utilisation du GPU, valeur recommand?e

	SDL_RenderPresent(rendu);

	bool continuer = true;   //bool?en fin de programme
	SDL_Event event;//gestion des ?v?nements souris/clavier, 
					//SDL_Event est de type struct
	while (continuer)
	{
		SDL_WaitEvent(&event);//attente d?un ?v?nement
		switch (event.type) //test du type d??v?nement
		{
		case SDL_QUIT: //clic sur la croix de fermeture
					   //on peut enlever SDL_Delay
			continuer = false;
			break;
		}
	}
	//destruction du renderer ? la fin
	SDL_DestroyRenderer(rendu);

	//destruction ? la fin
	SDL_DestroyWindow(win);   //equivalent du delete

	//fermeture
	SDL_Quit();
}
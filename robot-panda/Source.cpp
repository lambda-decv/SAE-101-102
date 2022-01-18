#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string> 
#include "config_sdl.h"

// Inclusion des headers
#include "fonctions.h"


using namespace std;

const int LARGEUR = 1200;
const int HAUTEUR = 575;



int main(int argc, char* argv[]) {
	srand(time(NULL));
	coord co,robotCo;
	co.x = 1;
	co.y = HAUTEUR - 50;
	robotCo.x = 400;
	robotCo.y = HAUTEUR;

	int moyenne = 0;
	int taille_max_atteinte = 0;
	int jour = 1;

	TTF_Init();
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Echec à l’ouverture";
		return 1;
	}

	//on crée la fenêtre
	SDL_Window* win = SDL_CreateWindow("Ma fenetre",
		SDL_WINDOWPOS_CENTERED,     //pos. X: autre option: SDL_WINDOWPOS_UNDEFINED
		SDL_WINDOWPOS_CENTERED,     //pos. Y: autre option: SDL_WINDOWPOS_UNDEFINED 
		LARGEUR, //largeur en pixels 
		HAUTEUR, //hauteur en pixels
		SDL_WINDOW_SHOWN //d’autres options (plein ecran, resizable, sans bordure...)
	);
	if (win == NULL)
		cout << "erreur ouverture fenetre";

	//Création d’un dessin associé à la fenêtre (1 seul renderer par fenetre)
	SDL_Renderer* rendu = SDL_CreateRenderer(
		win,  //nom de la fenêtre
		-1, //par défaut
		SDL_RENDERER_ACCELERATED); //utilisation du GPU, valeur recommandée

	init_bambous(bambous, TAILLE);
	SDL_Rect fond{ 800, 0, 400, 480 };
	SDL_SetRenderDrawColor(rendu, 255,255,255,255);
	SDL_RenderFillRect(rendu, &fond);
	SDL_RenderPresent(rendu);

	SDL_Surface* image = IMG_Load("sol.png");
	SDL_Texture* pTextureImage = SDL_CreateTextureFromSurface(rendu, image);
	SDL_FreeSurface(image);

	SDL_Surface* image2 = IMG_Load("soleil.png");
	SDL_Texture* pTextureImage2 = SDL_CreateTextureFromSurface(rendu, image2);
	SDL_FreeSurface(image2);

	SDL_Surface* robot = IMG_Load("panda-small.png");
	SDL_Texture* pTextureRobot = SDL_CreateTextureFromSurface(rendu, robot);
	SDL_FreeSurface(robot);

	SDL_Surface* boutond = IMG_Load("bouton droit.png");
	SDL_Texture* pTextureBoutonD = SDL_CreateTextureFromSurface(rendu, boutond);
	SDL_FreeSurface(boutond);



	affichageBg(rendu,pTextureImage,pTextureImage2,pTextureBoutonD);
	affichageRobot(rendu, robotCo,pTextureRobot);
	graph1(rendu);
	graph2(rendu);
	SDL_RenderPresent(rendu);


	bool continuer = true;
	bool press = false;
	SDL_Event event;

	while (continuer)
	{
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
					continuer = false;
					break;
				}
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					menu(rendu,font);
					press = true;
					break;
					// cases for other keypresses
				
				case SDLK_m:
					cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot,pTextureBoutonD,1);
					break;
				case SDLK_f:
					cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot,pTextureBoutonD, 2);
					break;

				break;
				// cases for other events
				}
			}
		}
	}
	SDL_DestroyTexture(pTextureImage);
	SDL_DestroyTexture(pTextureRobot);
	SDL_DestroyTexture(pTextureImage2);
	//destruction du renderer à la fin

	SDL_DestroyRenderer(rendu);

	//destruction à la fin
	SDL_DestroyWindow(win);   //equivalent du delete

	//fermeture
	SDL_Quit();
	return 0;
}

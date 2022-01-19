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
	
	int indice_panda = TAILLE - 1;

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

	SDL_Surface* boutond = IMG_Load("bouton droit.jpg");
	SDL_Texture* pTextureBoutonD = SDL_CreateTextureFromSurface(rendu, boutond);
	SDL_FreeSurface(boutond);

	SDL_Surface* boutong = IMG_Load("bouton gauche.jpg");
	SDL_Texture* pTextureBoutonG = SDL_CreateTextureFromSurface(rendu, boutong);
	SDL_FreeSurface(boutong);

	SDL_Surface* boutonc = IMG_Load("ciseaux.jpg");
	SDL_Texture* pTextureBoutonC = SDL_CreateTextureFromSurface(rendu, boutonc);
	SDL_FreeSurface(boutonc);

	affichageBg(rendu,pTextureImage,pTextureImage2,pTextureBoutonD,pTextureBoutonG);
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
			case SDL_MOUSEBUTTONUP://appui souris
				if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
					if (event.button.x > 725 && event.button.x<725 + 50 && event.button.y>505 && event.button.y < 505 + 50) { 		
						affichageBg(rendu, pTextureImage, pTextureImage2, pTextureBoutonD, pTextureBoutonD);
						boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
						boutoncouper(rendu, pTextureBoutonC);
						dessinComplet(bambous, rendu, TAILLE, co);
						deplaceravecboutonD(indice_panda, bambous, rendu, pTextureRobot);
					}
					SDL_RenderPresent(rendu);//on rafraichit
					if (event.button.x > 650 && event.button.x < 650 + 50 && event.button.y>505 && event.button.y < 505 + 50) { 
						affichageBg(rendu, pTextureImage, pTextureImage2, pTextureBoutonD, pTextureBoutonD);
						boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
						boutoncouper(rendu, pTextureBoutonC);
						dessinComplet(bambous, rendu, TAILLE, co);
						deplaceravecboutonG(indice_panda, bambous, rendu, pTextureRobot);
					}
					if (event.button.x > 575 && event.button.x < 575 + 50 && event.button.y>505 && event.button.y < 505 + 50) {
						affichageBg(rendu, pTextureImage, pTextureImage2, pTextureBoutonD, pTextureBoutonD);
						boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
						boutoncouper(rendu, pTextureBoutonC);
						dessinComplet(bambous, rendu, TAILLE, co);
						couperBambou(bambous, indice_panda);
						affichageRobot(rendu, bambous[indice_panda - 1].pos, pTextureRobot);
					}
					SDL_RenderPresent(rendu);//on rafraichit
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					menu(rendu,font);
					press = true;
					break;
					// cases for other keypresses
				
				case SDLK_m:
					cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot,pTextureBoutonD,1, pTextureBoutonG,pTextureBoutonC);
					break;
				case SDLK_f:
					cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot,pTextureBoutonD, 1, pTextureBoutonG,pTextureBoutonC);
					cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot,pTextureBoutonD, 2, pTextureBoutonG,pTextureBoutonC);
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

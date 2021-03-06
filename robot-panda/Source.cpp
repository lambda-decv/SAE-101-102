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

char* inputTextX;


Uint32 frameStart;
int frametime;
int fpsDelay = 0;

const int FPS = 60;
const int FRAMEDELAY = 1000 / FPS;

bool modeAuto = false;
bool reduceFasterEnable = false;

int main(int argc, char* argv[]) {
	srand(time(NULL));
	coord co, robotCo;
	co.x = 1;
	co.y = HAUTEUR - 50;
	robotCo.x = 400;
	robotCo.y = HAUTEUR;

	int moyenne = 0;
	int taille_max_atteinte = 0;
	int jour = 1;
	char nomBase[20] = "Choix Mode";
	bool verifTxtChangeMod = false;
	char nom[20] = "Reduce Max";
	char nom2[20] = "Reduce Fastest";


	int indice_panda = TAILLE - 1;

	TTF_Init();
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Echec à l’ouverture";
		return 1;
	}

	//on crée la fenêtre
	SDL_Window* win = SDL_CreateWindow("Robot Panda",
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
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
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

	SDL_Surface* boutonclosemenu = IMG_Load("croix.jpg");
	SDL_Texture* pTextureBoutonCloseMenu = SDL_CreateTextureFromSurface(rendu, boutonclosemenu);
	SDL_FreeSurface(boutonclosemenu);

	SDL_Surface* boutonp = IMG_Load("plus.jpg");
	SDL_Texture* pTextureBoutonP = SDL_CreateTextureFromSurface(rendu, boutonp);
	SDL_FreeSurface(boutonp);

	affichageBg(rendu, pTextureImage, pTextureImage2, pTextureBoutonD, pTextureBoutonG, pTextureBoutonC);
	affichageRobot(rendu, robotCo, pTextureRobot, indice_panda);
	rectBouton(rendu);
	boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
	boutoncouper(rendu, pTextureBoutonC);
	boutonplus(rendu, pTextureBoutonP);
	affichageTxtPlay(rendu, font);
	affichageTxtPause(rendu, font);
	affichageTxtChangeMod(rendu, font, nom);
	affichageRobot(rendu, robotCo, pTextureRobot, indice_panda);
	graph1(rendu);
	graph2(rendu);
	dessinComplet(bambous, rendu, TAILLE, co);
	battery(rendu, 7);
	SDL_RenderPresent(rendu);


	bool continuer = true;
	bool press = false;
	SDL_Event event;

	while (continuer)
	{
		frameStart = SDL_GetTicks();
		fpsDelay++;
		if (modeAuto == true && fpsDelay >= 30) {
			cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot, pTextureBoutonD, 1, pTextureBoutonG, pTextureBoutonC);
			fpsDelay = 0;
		}
		frametime = SDL_GetTicks() - frameStart;
		if (FRAMEDELAY > frametime) {
			SDL_Delay(FRAMEDELAY - frametime);
		}
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
					continuer = false;
					break;
				}
			case SDL_MOUSEBUTTONUP://appui souris
				if (event.button.button == SDL_BUTTON_LEFT) {
					// FLECHE DROITE
					if (event.button.x > 725 && event.button.x < 725 + 50 && event.button.y>485 && event.button.y < 485 + 50) {
						affichageBg(rendu, pTextureImage, pTextureImage2, pTextureBoutonD, pTextureBoutonG, pTextureBoutonC);
						boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
						boutoncouper(rendu, pTextureBoutonC);
						dessinComplet(bambous, rendu, TAILLE, co);
						deplaceravecboutonD(indice_panda, bambous, rendu, pTextureRobot);
						SDL_RenderPresent(rendu);
					}
					// FLECHE GAUCHE
					if (event.button.x > 650 && event.button.x < 650 + 50 && event.button.y>485 && event.button.y < 485 + 50) {
						affichageBg(rendu, pTextureImage, pTextureImage2, pTextureBoutonD, pTextureBoutonG, pTextureBoutonC);
						boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
						boutoncouper(rendu, pTextureBoutonC);
						dessinComplet(bambous, rendu, TAILLE, co);
						deplaceravecboutonG(indice_panda, bambous, rendu, pTextureRobot);
						SDL_RenderPresent(rendu);
					}
					//BOUTON COUPE
					if (event.button.x > 575 && event.button.x < 575 + 50 && event.button.y>505 && event.button.y < 505 + 50) {
						affichageBg(rendu, pTextureImage, pTextureImage2, pTextureBoutonD, pTextureBoutonG, pTextureBoutonC);
						boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
						boutoncouper(rendu, pTextureBoutonC);
						dessinComplet(bambous, rendu, TAILLE, co);
						couperBambou(bambous, indice_panda);
						affichageRobot(rendu, bambous[indice_panda].pos, pTextureRobot, indice_panda);
						cout << indice_panda;
					}
					// BOUTON PLAY
					if (event.button.x > 968 && event.button.x < 968 + 200 && event.button.y>488 && event.button.y < 488 + 35) {
						if (modeAuto == false) {
							modeAuto = true;
						}
					}
					//BOUTON PAUSE
					if (event.button.x > 968 && event.button.x < 968 + 200 && event.button.y>532 && event.button.y < 532 + 35) {
						if (modeAuto == true) {
							modeAuto = false;
						}
					}
					// BOUTON Change Mode 
					if (event.button.x > 132 && event.button.x < 132 + 200 && event.button.y>488 && event.button.y < 488 + 35) {
						SDL_Rect maxValue;
						maxValue.x = 32;
						maxValue.y = 532;
						maxValue.w = 200;
						maxValue.h = 35;

						SDL_SetRenderDrawColor(rendu, 0, 0, 255, 255);
						SDL_RenderDrawRect(rendu, &maxValue);
						SDL_RenderFillRect(rendu, &maxValue);

						SDL_Rect fastestValue;
						fastestValue.x = 242;
						fastestValue.y = 532;
						fastestValue.w = 200;
						fastestValue.h = 35;

						SDL_SetRenderDrawColor(rendu, 0, 0, 255, 255);
						SDL_RenderDrawRect(rendu, &fastestValue);
						SDL_RenderFillRect(rendu, &fastestValue);



						SDL_Color noir = { 0,0,0 };
						SDL_Rect positionTexteMaxValue;
						SDL_Rect positionTexteFastestValue;

						SDL_Texture* texture = loadText(rendu, nom, noir, font);
						SDL_Texture* texture2 = loadText(rendu, nom2, noir, font);

						positionTexteMaxValue.x = 55;
						positionTexteMaxValue.y = 535;

						positionTexteFastestValue.x = 250;
						positionTexteFastestValue.y = 535;

						SDL_QueryTexture(texture, NULL, NULL, &positionTexteMaxValue.w, &positionTexteMaxValue.h);
						SDL_QueryTexture(texture2, NULL, NULL, &positionTexteFastestValue.w, &positionTexteFastestValue.h);

						positionTexteMaxValue.w *= 1.2;
						positionTexteMaxValue.h *= 1.2;

						positionTexteFastestValue.w *= 1.2;
						positionTexteFastestValue.h *= 1.2;

						SDL_RenderCopy(rendu, texture, NULL, &positionTexteMaxValue);
						SDL_RenderCopy(rendu, texture2, NULL, &positionTexteFastestValue);

						SDL_DestroyTexture(texture);
						SDL_DestroyTexture(texture2);
						SDL_RenderPresent(rendu);
					}
					// Si on appuie sur BTN Max Value
					if (event.button.x > 32 && event.button.x < 32 + 200 && event.button.y>532 && event.button.y < 532 + 35) {
						if (reduceFasterEnable == false) {
							cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot, pTextureBoutonD, 1, pTextureBoutonG, pTextureBoutonC);
						}
					}
					// BOUTON reduce Faster
					if (event.button.x > 242 && event.button.x < 242 + 200 && event.button.y>532 && event.button.y < 532 + 35) {
						reduceFasterEnable = true;
						cleanBoutonMaxValue(rendu);
						affichageTxtValueX(rendu, font);
						cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot, pTextureBoutonD, 2, pTextureBoutonG, pTextureBoutonC);

					}
					//// Bouton Menu
					//if (event.button.x > largeur / 2 - 200 && event.button.y > hauteur - 60 && event.button.x < largeur / 2 + 200 && event.button.y < hauteur) {
					//	SDL_RenderClear(rendu);
					//	menu(rendu);
					//	boutonCloseMenu(rendu, pTextureBoutonCloseMenu);
					//}
					// BOUTON Fermeture menu

				}
				break;
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
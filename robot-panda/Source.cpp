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

	affichageBg(rendu,pTextureImage,pTextureImage2,pTextureBoutonD,pTextureBoutonG,pTextureBoutonC);
	affichageRobot(rendu, robotCo,pTextureRobot);
	rectBouton(rendu);
	boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
	boutoncouper(rendu, pTextureBoutonC);
	affichageTxtPlay(rendu,font);
	affichageTxtPause(rendu, font);
	affichageTxtChangeMod(rendu, font, nomBase);
	//affichageTxtValueX(rendu, font);
	graph1(rendu);
	graph2(rendu);
	battery(rendu, 7);

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
					if (event.button.x > 725 && event.button.x < 725 + 50 && event.button.y>505 && event.button.y < 505 + 50) {
						affichageBg(rendu, pTextureImage, pTextureImage2, pTextureBoutonD, pTextureBoutonG, pTextureBoutonC);
						boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
						boutoncouper(rendu, pTextureBoutonC);
						dessinComplet(bambous, rendu, TAILLE, co);
						deplaceravecboutonD(indice_panda, bambous, rendu, pTextureRobot);
					}
					SDL_RenderPresent(rendu);//on rafraichit
					if (event.button.x > 650 && event.button.x < 650 + 50 && event.button.y>505 && event.button.y < 505 + 50) {
						affichageBg(rendu, pTextureImage, pTextureImage2, pTextureBoutonD, pTextureBoutonG, pTextureBoutonC);
						boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
						boutoncouper(rendu, pTextureBoutonC);
						dessinComplet(bambous, rendu, TAILLE, co);
						deplaceravecboutonG(indice_panda, bambous, rendu, pTextureRobot);
					}
					SDL_RenderPresent(rendu);//on rafraichit
					if (event.button.x > 575 && event.button.x < 575 + 50 && event.button.y>505 && event.button.y < 505 + 50) {
						affichageBg(rendu, pTextureImage, pTextureImage2, pTextureBoutonD, pTextureBoutonG, pTextureBoutonC);
						boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
						boutoncouper(rendu, pTextureBoutonC);
						dessinComplet(bambous, rendu, TAILLE, co);
						couperBambou(bambous, indice_panda);
						affichageRobot(rendu, bambous[indice_panda].pos, pTextureRobot);
					}
					SDL_RenderPresent(rendu);//on rafraichit
					if (event.button.x > 968 && event.button.x < 968 + 200 && event.button.y>488 && event.button.y < 488 + 35) {
						while (continuer = true){
							if (event.button.x > 968 && event.button.x < 968 + 200 && event.button.y>532 && event.button.y < 532 + 35){ cout << "click"; }
							cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot, pTextureBoutonD, 1, pTextureBoutonG, pTextureBoutonC);
							Sleep(500);
						}
					}
					SDL_RenderPresent(rendu);//on rafraichit
					if (event.button.x > 32 && event.button.x < 32 + 200 && event.button.y>488 && event.button.y < 488 + 35) {
						affichageBg(rendu, pTextureImage, pTextureImage2, pTextureBoutonD, pTextureBoutonG, pTextureBoutonC);
						boutonsdirection(rendu, pTextureBoutonD, pTextureBoutonG);
						boutoncouper(rendu, pTextureBoutonC);
						dessinComplet(bambous, rendu, TAILLE, co);
						couperBambou(bambous, indice_panda);
						affichageRobot(rendu, bambous[indice_panda - 1].pos, pTextureRobot);
					}
					SDL_RenderPresent(rendu);//on rafraichit
					if (event.button.x > 32 && event.button.x < 32 + 200 && event.button.y>488 && event.button.y < 488 + 35) {

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
					if (event.button.x > 32 && event.button.x < 32 + 200 && event.button.y>532 && event.button.y < 532 + 35) {
						cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot, pTextureBoutonD, 1, pTextureBoutonG, pTextureBoutonC);
					}
					if (event.button.x > 242 && event.button.x < 242 + 200 && event.button.y>532 && event.button.y < 532 + 35) {
						cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot, pTextureBoutonD, 2, pTextureBoutonG, pTextureBoutonC);
					}
					SDL_RenderPresent(rendu);//on rafraichit
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					menu(rendu, font);
					press = true;
					break;
					// cases for other keypresses

				case SDLK_m:
					cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot, pTextureBoutonD, 1, pTextureBoutonG, pTextureBoutonC);
					break;
				case SDLK_f:
					cycleJournalier(rendu, bambous, co, pTextureImage, pTextureImage2, pTextureRobot, pTextureBoutonD, 2, pTextureBoutonG, pTextureBoutonC);
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

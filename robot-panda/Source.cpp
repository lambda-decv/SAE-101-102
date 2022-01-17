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
const int HAUTEUR = 480;



int main(int argc, char* argv[]) {
	coord co,robotCo;
	co.x = 10;
	co.y = HAUTEUR - 50;
	robotCo.x = 400;
	robotCo.y = HAUTEUR;

	int moyenne = 0;
	int taille_max_atteinte = 0;
	int jour = 3;
    init_bambous(bambous, TAILLE);
	affichgeBambous(bambous, TAILLE);
	


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

	SDL_RenderPresent(rendu);
	for (int i = 0; i < jour; i++) {
		cout << "=============== Cycle = " << i << " ===============" << endl;
		croissance(bambous, TAILLE);
		affichgeBambous(bambous, TAILLE);
		couperBambou(bambous, TAILLE, rendu);
		affichgeBambous(bambous, TAILLE);
	}

	bool continuer = true;   //booléen fin de programme
	SDL_Event event;//gestion des évènements souris/clavier, 
					//SDL_Event est de type struct

	SDL_Surface* image = IMG_Load("sol.png");
	SDL_Texture* pTextureImage = SDL_CreateTextureFromSurface(rendu, image);
	SDL_FreeSurface(image);
	SDL_Rect src1{ 0, 0, 0, 0 };
	SDL_Rect dst1{ 0, 380, 800, 100 };
	SDL_QueryTexture(pTextureImage, nullptr, nullptr, &src1.w, &src1.h);


	SDL_Surface* image2 = IMG_Load("soleil.png");
	SDL_Texture* pTextureImage2 = SDL_CreateTextureFromSurface(rendu, image2);
	SDL_FreeSurface(image2);
	SDL_Rect src2{ 0, 0, 0, 0 };
	SDL_Rect dst2{ 675, 30, 75, 75 };
	SDL_QueryTexture(pTextureImage2, nullptr, nullptr, &src2.w, &src2.h);




	SDL_Surface* robot = IMG_Load("panda-small.png");
	SDL_Texture* pTextureRobot = SDL_CreateTextureFromSurface(rendu, robot);
	SDL_FreeSurface(robot);

	SDL_Rect rectangle{ 800, 0, 400, 480 };

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
		SDL_RenderCopy(rendu, pTextureImage2, &src2, &dst2); // Affiche la texture entièrement

		dessinComplet(bambous, rendu, TAILLE, co);
		affichageRobot(rendu, robot, pTextureRobot, robotCo);
		affichageRobot(rendu, robot, pTextureRobot, robotCo); // Affiche la texture entièrement
		SDL_RenderCopy(rendu, pTextureImage2, &src2, &dst2); // Affiche la texture entièrement

		SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
		SDL_RenderFillRect(rendu, &rectangle);
		

		SDL_RenderPresent(rendu);
	}
	
	SDL_RenderPresent(rendu);

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
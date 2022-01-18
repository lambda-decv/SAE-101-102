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
	srand(time(NULL));
	coord co, robotCo;
	co.x = 10;
	co.y = HAUTEUR - 50;
	robotCo.x = 400;
	robotCo.y = HAUTEUR;

	int moyenne = 0;
	int taille_max_atteinte = 0;
	int jour = 1;



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
	cycleJournalier(rendu, bambous, 5, co);


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

	SDL_SetRenderDrawColor(rendu, 0, 242, 255, 255);
	SDL_RenderClear(rendu);

	SDL_RenderCopy(rendu, pTextureImage, &src1, &dst1); // Affiche la texture entièrement
	SDL_RenderCopy(rendu, pTextureImage2, &src2, &dst2); // Affiche la texture entièrement

	affichageRobot(rendu, robot, pTextureRobot, robotCo);
	affichageRobot(rendu, robot, pTextureRobot, robotCo); // Affiche la texture entièrement
	SDL_RenderCopy(rendu, pTextureImage2, &src2, &dst2); // Affiche la texture entièrement

	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderFillRect(rendu, &rectangle);


	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA.x, pointA.y, pointB.x, pointB.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA2.x, pointA2.y, pointB2.x, pointB2.y);

	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA3.x, pointA3.y, pointB3.x, pointB3.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA4.x, pointA4.y, pointB4.x, pointB4.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA5.x, pointA5.y, pointB5.x, pointB5.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA6.x, pointA6.y, pointB6.x, pointB6.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA7.x, pointA7.y, pointB7.x, pointB7.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA8.x, pointA8.y, pointB8.x, pointB8.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA9.x, pointA9.y, pointB9.x, pointB9.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA10.x, pointA10.y, pointB10.x, pointB10.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA11.x, pointA11.y, pointB11.x, pointB11.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA12.x, pointA12.y, pointB12.x, pointB12.y);
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
	SDL_RenderDrawLine(rendu, pointA13.x, pointA13.y, pointB13.x, pointB13.y);


	dessinComplet(bambous, rendu, TAILLE, co);

	SDL_RenderPresent(rendu);

	while (continuer)
	{
		SDL_WaitEvent(&event);//attente d’un évènement
		switch (event.type) //test du type d’évènement
		{
		case SDL_QUIT: //clic sur la croix de fermeture
		   //on peut enlever SDL_Delay
			continuer = false;
			break;
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

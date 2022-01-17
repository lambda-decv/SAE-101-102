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
	int moyenne = 0;
	int taille_max_atteinte = 0;
	int jour = 1;
    init_bambous(bambous, TAILLE);
	cycleJour(jour);
	affichgeBambous(bambous, TAILLE);
	taille_max(bambous, TAILLE, taille_max_atteinte);
	moy(bambous, TAILLE, moyenne);
	afficherStat(bambous, TAILLE, moyenne, taille_max_atteinte);


        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
            return EXIT_FAILURE;
        }

        SDL_Window* pWindow{ nullptr };
        SDL_Renderer* pRenderer{ nullptr };

        if (SDL_CreateWindowAndRenderer(LARGEUR, HAUTEUR, SDL_WINDOW_SHOWN, &pWindow, &pRenderer) < 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
            SDL_Quit();
            return EXIT_FAILURE;
        }

        SDL_Event events;
        bool isOpen{ true };

        SDL_Surface* image = IMG_Load("sol.png");
        SDL_Texture* pTextureImage = SDL_CreateTextureFromSurface(pRenderer, image);

        SDL_FreeSurface(image);
        SDL_Rect src1{ 0, 0, 0, 0 };
        SDL_Rect src2{ 0, 0, 0, 0 };

        SDL_Rect dst1{ 0, 380, 800, 100 };

        SDL_QueryTexture(pTextureImage, nullptr, nullptr, &src1.w, &src1.h);

        // Récuperer les bons coordonnées x et y pour afficher la tête du tigre
        src2.x = src1.w / 2 - 30;
        src2.y = src1.h / 2 - 50;
        src2.w = 50;
        src2.h = 50;

	SDL_RenderPresent(rendu);
	coord coordonnees;
	coordonnees.x = 5;
	coordonnees.y = HAUTEUR;
	dessinComplet(bambous,rendu,TAILLE,coordonnees);

        while (isOpen)
        {
            while (SDL_PollEvent(&events))
            {
                switch (events.type)
                {
                case SDL_QUIT:
                    isOpen = false;
                    break;
                }
            }


            SDL_SetRenderDrawColor(pRenderer, 0, 242, 255, 255);
            SDL_RenderClear(pRenderer);

            SDL_RenderCopy(pRenderer, pTextureImage, &src1, &dst1); // Affiche la texture entièrement

            SDL_RenderPresent(pRenderer);
        }

        SDL_DestroyTexture(pTextureImage);
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();

        return EXIT_SUCCESS;
}


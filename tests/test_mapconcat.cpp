#include "MapConcat/Overworld.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

bool init_sdl(SDL_Window*, SDL_Renderer*, int w, int h);

int main () {
	/*
	int window_width = 640;
	int window_height = 480;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	
	if(init_sdl(window, renderer, window_width, window_height)) {
		Overworld overworld(8, 5);

		bool quit = false;
		SDL_Event event;

		while (!quit) {
			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT) quit = true;
				if( event.type == SDL_KEYUP && event.key.repeat == 0 ) {
					switch (event.key.keysym.sym) {
							case SDLK_w :
								overworld.MoveY(-1); break;
							case SDLK_a :
								overworld.MoveX(-1); break;
							case SDLK_s :
								overworld.MoveY(1); break;
							case SDLK_d :
								overworld.MoveY(1); break;
							default:
								break;
					}
				}
			}
		}
		SDL_DestroyWindow(window);
		SDL_Quit();
		printf("shutdown success\n");
	}
	*/

	//return 0;
	Overworld overworld(8, 5);

	char i = 'w';
	while (i!= 'q') {
		std::cin >> i;
		switch (i) {
		case 'w':
            overworld.MoveY(-1);
			std::cout << "up\n";
			break;
		case 's':
            overworld.MoveY(1);
			std::cout << "down\n";
			break;
		case 'a':
            overworld.MoveX(-1);
			std::cout << "left\n";
			break;
		case 'd':
            overworld.MoveX(1);
			std::cout << "right\n";
			break;
		case 'p':
            overworld.map.PrintAsciiMap();
			break;
		}
	}

	return 0;
}

bool init_sdl(SDL_Window* window, SDL_Renderer* renderer,int w, int h) {
	/* init sdl */
	if (SDL_Init( SDL_INIT_VIDEO) < 0 ) {
		printf("SDL initalisation failure: %s\n", SDL_GetError());
		return false;
	}

	/* init window */
	window = SDL_CreateWindow("Test",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 
			w, h,
			SDL_WINDOW_SHOWN);

	if (window == NULL) {
		printf("SDL failed window creation: %s\n", SDL_GetError());
		return false;
	}

	/* init renderer */
	renderer = SDL_CreateRenderer(
			window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) {
		printf("SDL failed to create renderer: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	/* init sdl image */ 
	int img_flag = IMG_INIT_PNG;
	if ( !(IMG_Init(img_flag) & img_flag)) {
		printf("Failed to initalise SDL_IMG: %s\n", IMG_GetError());
		return false;
	}

	return true; 
}

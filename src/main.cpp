#include "stdafx.h"
#include <string>
#include "chip8.h"
#include <SDL.h>
#include <iostream>
#include <fstream>

void drawGraphics(const Chip8& chip8, SDL_Window* window, SDL_Renderer* renderer) {
	// Set render color to black and clear screen with this color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Set render color to white (rect will be rendered in this color)
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (int i = 0; i < chip8.SCREEN_HEIGHT; i++) {
		for (int j = 0; j < chip8.SCREEN_WIDTH; j++) {
			if (chip8.gfx[i][j] == 1) {
				// Creat a rect at pos (j * 10, i * 10) that's 10 pixels wide and 10 pixels high.
				SDL_Rect r = { j * 10, i * 10, 10, 10 };

				// Render rect
				if (SDL_RenderFillRect(renderer, &r) == -1) {
					printf("There's an error with SDL_RenderFillRect");
				}
			}
		}
	}

	SDL_RenderPresent(renderer);
}

//Screen dimension constants
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 320;

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

int _tmain(int argc, _TCHAR* argv[]) {
	Chip8 chip8;

	// Initialize the Chip8 system and load the game into the memory
	chip8.initialize();
	chip8.loadGame();

	// Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

	// The window we'll be rendering to
    std::string windowName = "c8cpp - " + chip8.gameName;
	SDL_Window* window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	// Main loop flag
	bool quit = false;

	// Setup renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// Event handler
	SDL_Event e;

	// main emulation loop
	while (!quit) {
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			// User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			// User presses a key
			else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
				chip8.handleKey(e);
			}
		}

		// Emulate one cycle
		chip8.emulateCycle();

		// If the draw flag is set, update the screen
		if (chip8.drawFlag) {
			drawGraphics(chip8, window, renderer);
		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
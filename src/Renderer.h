#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "interfaces/Drawable.h"

namespace config {
// GUI window.
//constexpr int WINDOW_WIDTH {640};
//constexpr int WINDOW_HEIGHT {480};

// Game rendering params.
constexpr int TILE_DIM = 32;
constexpr int TILE_PER_COL = 20;
constexpr int TILE_PER_ROW = 20;

constexpr int TILE_PER_COL_SCREEN = 20;
constexpr int TILE_PER_ROW_SCREEN = 20;

constexpr int SCREEN_WIDTH = TILE_PER_COL_SCREEN * TILE_DIM;
constexpr int SCREEN_HEIGHT = TILE_PER_ROW_SCREEN * TILE_DIM;

//constexpr int WINDOW_WIDTH = 40 * TILE_DIM;
//constexpr int WINDOW_HEIGHT = 20 * TILE_DIM;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int SCREEN_OFFSET_X;
extern int SCREEN_OFFSET_Y;

void RecalculateWindowVariables(int w = ::config::SCREEN_WIDTH, int h = ::config::SCREEN_HEIGHT, int x = 0, int y = 0);
} /* config */

namespace sdl {
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern float deltatime;
} /* sdl */


namespace render {
Drawable *AddToRenderStack(Drawable *);
bool RemoveFromRenderStack(Drawable *);
void DrawStack();

}
#include <iostream>

#include <SDL_image.h>
#include <cstdio>

#include "Renderer.h"
#include "InputContext.h"
#include "UI/dialog.h"
#include "UI/menu.h"

#include "Map.h"
#include "drawable/CharacterPlayer.h"

#include "input/player.h"
#include "input/editor.h"
#include "Game.h"

namespace sdl {

  SDL_Window *window{nullptr};
  SDL_Renderer *renderer{nullptr};
  float deltatime = 0;
  FrameConfig *g_frame_config{nullptr};

/* load sdl resources */
  bool SdlLoad(FrameConfig &frame_config) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      printf("SDL initialisation failure: %s\n", SDL_GetError());
      return false;
    }
//    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Hello world", "test description", sdl::window);

    /* Init window */
    window = SDL_CreateWindow("Poogymon",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              frame_config.GetWidth(),
                              frame_config.GetHeight(),
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    frame_config.RecalculateWindowVariables(DimensionType{
        .width =frame_config.GetWidth(),
        .height = frame_config.GetHeight(),
    });

    if (window == nullptr) {
      printf("SDL failed window creation: %s\n", SDL_GetError());
      return false;
    }

    /* Init renderer */
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) {
      printf("SDL failed to create renderer: %s\n", SDL_GetError());
      return false;
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    /* Init sdl image */
    int img_flag = IMG_INIT_PNG;

    if (!(IMG_Init(img_flag) & img_flag)) {
      printf("Failed to initalise SDL_IMG: %s\n", IMG_GetError());
      return false;
    }

    return true;
  }

  void Cleanup() {
    SDL_DestroyRenderer(sdl::renderer);
    SDL_DestroyWindow(sdl::window);

    IMG_Quit();
    SDL_Quit();
    puts("shutdown success");
  }

} /* sdl */

void Start(FrameConfig &frame_config) {
  auto game = Game(frame_config);
  game.Start();
}

int main() {
  FrameConfig frame_config;
  sdl::g_frame_config = &frame_config;

  if (!sdl::SdlLoad(frame_config)) {
    return 1;
  }

  Start(frame_config);
  sdl::Cleanup();
  return 0;
}

namespace render {
  std::vector<Drawable *> stack{};

  Drawable *AddToRenderStack(Drawable *drawable) {
    stack.push_back(drawable);
    (void) puts("added to render stack");
    return drawable;
  }

  bool RemoveFromRenderStack(Drawable *p_drawable) {
    auto found = std::find(std::begin(stack), std::end(stack), p_drawable);

    if (*found) {
      stack.erase(found);
    }

    return true;
  }

  void DrawStack() {
    for (Drawable *item: stack) {
      item->Render();
    }
  }
} /* namespace render */

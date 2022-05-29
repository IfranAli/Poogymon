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

namespace sdl {

  SDL_Window *window{nullptr};
  SDL_Renderer *renderer{nullptr};
  float deltatime = 0;

/* load sdl resources */
  bool SdlLoad() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      printf("SDL initialisation failure: %s\n", SDL_GetError());
      return false;
    }

//  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Hello world", "test description", sdl::window);

    config::RecalculateWindowVariables(::config::SCREEN_WIDTH, ::config::SCREEN_HEIGHT);
    /* Init window */
    window = SDL_CreateWindow("Poogymon",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              config::WINDOW_WIDTH,
                              config::WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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

void Start() {
  bool quit = false;
  SDL_Event event;

  /*
  int countedFrames = 0;
  float avgFPD = 0;
  LTimer timer;
  timer.Start();
  */

  SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

  /*
   * Not really a context but rather just a class that initialises
   * a bunch on input maps since they can't be done compile time.
   */
  input::InputContext ic;

  /* Initialise subsystems */
  if (!dialog::init()) {
    quit = true;
    std::cerr << "Failed to Init dialog subsystem " << SDL_GetError() << "\n";
  }

  const std::string MASTER_FILE_NAME = "01master.map";
  map::Map m_map(MASTER_FILE_NAME);
  map::active_map = &m_map;

  CharacterPlayer player;
  player::player = &player;

  sdl::deltatime = 0;
  float start_ticks;
  dialog::add_item(sdl::deltatime, 0, 200);

  input::SetInputHandler(player::ih_player);

  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      switch (event.type) {
        case SDL_WINDOWEVENT:
          if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            config::RecalculateWindowVariables(event.window.data1, event.window.data2);
          }
          break;
        case SDL_QUIT:quit = true;
          break;
        default:input::InputContext::HandleInput(event);
      }
    }

    start_ticks = SDL_GetTicks();

    /* Render and display frame */
    SDL_SetRenderDrawColor(sdl::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(sdl::renderer);

    m_map.Tick();
    m_map.RenderToScreen();

    if (player::is_visible) {
      player::player->Tick();
    }

    render::DrawStack();
    dialog::update_labels(sdl::deltatime);
    menu::draw();
    editor::DrawTileset();

    SDL_RenderPresent(sdl::renderer);
    sdl::deltatime = (SDL_GetTicks() - start_ticks) / 1000.f;
  }

  dialog::close();
}

int main() {
  if (!sdl::SdlLoad()) {
    return 1;
  }

  Start();
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

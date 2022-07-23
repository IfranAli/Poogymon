//
// Created by Ila Luvox on 3/7/2022.
//

#include <SDL.h>
#include <string>
#include <iostream>
#include "Game.h"
#include "InputManager.h"
#include "UI/dialog.h"
#include "World/Map.h"
#include "Renderer.h"
#include "UI/menu.h"
#include "input/editor.h"
#include "input/player.h"

namespace {
  static const std::string MASTER_FILE_NAME = "01master.map";
}

Game::Game(FrameConfig &frame_config) :
    frame_config_(frame_config),
    map_(frame_config_),
    player_(frame_config_, &map_) {

  map::active_map = &map_;

  /* Initialise subsystems */
  if (!dialog::init()) {
    quit_ = true;
    std::cerr << "Failed to Init dialog subsystem " << SDL_GetError() << "\n";
  }
}

Game::~Game() = default;

void Game::Start() {
  input::InitInputMaps(render_manager_, map_, player_);
  sdl::deltatime = 0;
  dialog::add_item(sdl::deltatime, 0, 200);

  input::SetInputHandler(player::ih_player);

  GameLoop();

  dialog::close();
}

void Game::GameLoop() {
  float start_ticks = 0;
  SDL_Event event;
  SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

  while (!quit_) {
    while (SDL_PollEvent(&event) != 0) {
      switch (event.type) {
        case SDL_WINDOWEVENT:
          if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            frame_config_.RecalculateWindowVariables(DimensionType{
                .width= event.window.data1,
                .height = event.window.data2
            });
          }
          break;
        case SDL_QUIT:quit_ = true;
          break;
        default:input::InputManager::HandleInput(event);
          break;
      }
    }

    start_ticks = SDL_GetTicks();

    /* Render and display frame */
    SDL_SetRenderDrawColor(sdl::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(sdl::renderer);

    player_.Tick();

    render_manager_.DrawStack();
    dialog::update_labels(sdl::deltatime);
    menu::draw();
    editor::DrawTileset();

    SDL_RenderPresent(sdl::renderer);
    sdl::deltatime = (SDL_GetTicks() - start_ticks) / 1000.f;
  }
}


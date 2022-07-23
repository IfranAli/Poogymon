#pragma once

#include <stack>
#include <SDL.h>

#include "controls.h"
#include "Common.h"

namespace input {
  class InputManager {
   public:
    InputManager();
    ~InputManager();

    static void HandleInput(SDL_Event &e);
  };

  void SetInputHandler(InputHandler &handler);
  void SetPrevHandler();

  extern int mouse_x, mouse_y;
}

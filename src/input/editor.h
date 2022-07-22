#pragma once

#include "../Common.h"
#include "../RenderManager.h"
#include "../Map.h"

namespace editor {
  void Init(RenderManager *render_manager, map::Map *map);
  void OpenMenu();

  extern void DrawTileset();
  extern input::InputHandler editor;
}

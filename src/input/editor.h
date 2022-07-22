#pragma once

#include "../Common.h"
#include "../RenderManager.h"
#include "../Map.h"
#include "../drawable/CharacterPlayer.h"

namespace editor {
  void Init(RenderManager *render_manager, CharacterPlayer *, map::Map *map);
  void OpenMenu();

  extern void DrawTileset();
  extern input::InputHandler editor;
}

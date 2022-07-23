#pragma once
#include "../Common.h"
#include "../World/Map.h"
#include "../drawable/CharacterPlayer.h"
#include "../RenderManager.h"

namespace player {
  void Init(RenderManager *, map::Map *p_map, CharacterPlayer *p_player);

  extern input::InputHandler ih_player;
}

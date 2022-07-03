#pragma once
#include "../Common.h"
#include "../Map.h"
#include "../drawable/CharacterPlayer.h"

namespace player {
  void Init(map::Map *p_map, CharacterPlayer *p_player);

  extern input::InputHandler ih_player;
}

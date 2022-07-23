#pragma once

#include "Common.h"
#include "World/Map.h"
#include "drawable/CharacterPlayer.h"
#include "RenderManager.h"

namespace input {
  void InitInputMaps(RenderManager &, map::Map &, CharacterPlayer &player);
}


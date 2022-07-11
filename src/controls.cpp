#include "UI/menu.h"
#include "input/editor.h"
#include "input/player.h"
#include "input/text_select.h"
#include "drawable/CharacterPlayer.h"

namespace input {
  void InitInputMaps(
      map::Map *p_map,
      CharacterPlayer *p_character_player
  ) {
    menu::init_input_maps();
    editor::Init();
    player::Init(p_map, p_character_player);
    text_select::init();

    puts("Map initialization complete.");
  }

} /* input */

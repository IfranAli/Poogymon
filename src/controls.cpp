#include "UI/menu.h"
#include "input/editor.h"
#include "input/player.h"
#include "input/text_select.h"
#include "drawable/CharacterPlayer.h"
#include "RenderManager.h"

namespace input {
  void InitInputMaps(
      RenderManager &render_manager,
      map::Map &map,
      CharacterPlayer &character_player
  ) {
    menu::init_input_maps();
    editor::Init(&render_manager, &character_player, &map);
    player::Init(&render_manager, &map, &character_player);
    text_select::init();

    puts("Map initialization complete.");
  }

} /* input */

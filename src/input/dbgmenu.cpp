#include "dbgmenu.h"
#include "editor.h"
#include "player.h"
#include "text_select.h"
#include "../drawable/CharacterPlayer.h"
#include "../InputContext.h"
#include "../UI/menu.h"

#include <vector>
#include <string>

namespace debug {
  using namespace input;

  void open_editor() {
    SetInputHandler(editor::editor);
  }

  void open_text_select() {
    SetInputHandler(text_select::text_select);
  }

  void show_rnd() {
    std::vector<std::string> m{
        std::to_string(std::rand()),
        std::to_string(std::rand()),
        std::to_string(std::rand()),
        std::to_string(std::rand()),
        std::to_string(std::rand()),
    };

    menu::AddMenu(std::move(m), nullptr);
  }

  void nest_menu() {
    input_fn fn[]{
        []() { player::draw_bounds = !player::draw_bounds; },
        show_rnd,
        []() { puts("This is H"); },
        open_menu,
        []() { puts("This is J"); },
    };

    menu::AddMenu(
        {"BoxBounds", "Rand", "H", "NEST Menu", "J"}, fn
    );
  }

  void open_menu() {
    input_fn fn[]{
        []() { puts("This is A"); },
        nest_menu,
        []() { SetInputHandler(player::ih_player); },
        open_text_select,
        open_editor,
        editor::OpenMenu
    };

    menu::AddMenu({
                      "A",
                      "Nest Menu",
                      "Player",
                      "Text Select",
                      "Editor",
                      "Map Tools"
                  }, fn
    );

    input::SetInputHandler(menu::menu);
  }
}

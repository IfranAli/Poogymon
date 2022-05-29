#pragma once

#include "../Common.h"
#include "../controls.h"

#include <vector>
#include <string>

namespace menu {
  extern input::InputHandler menu;
  extern size_t current_menu;

  void init_input_maps();

  void AddMenu(std::vector<std::string> &&entries, input::input_fn fn[]);
  void draw();

  [[nodiscard]] size_t lastSelectedIndex();
  [[nodiscard]] std::string lastSelectedTitle();

  void menu_close();
  bool is_open();
}

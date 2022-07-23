#include "menu.h"
#include "../InputManager.h"
#include "../Texture.h"
#include "dialog.h"

#include <iostream>

namespace menu {
  unsigned char constexpr MENU_MAX_SIZE = 10;

  struct Menu {
    size_t selected{0};
    bool isEnabled{false};

    // TODO: allow metadata instead of just string.
    std::vector<std::string> names;
    input::input_fn fn[MENU_MAX_SIZE]{};
    dialog::menu_config config;
  };

  size_t current_menu = 0;
  std::vector<Menu> menus;

  bool is_open() {
    return menus.at(current_menu).isEnabled;
  }

  void AddMenu(std::vector<std::string> &&entries, input::input_fn fn[]) {
    if (entries.size() > MENU_MAX_SIZE) {
      puts("too many entries");
      return;
    }

    Menu &m = *menus.emplace(menus.end());
    m.names = std::move(entries);

    // TODO: Handle disabled/nullptr fn
    const size_t MAX = m.names.size();

    if (fn == nullptr) {
      for (size_t i = 0; i < MAX; ++i) {
        m.fn[i] = nullptr;
      }
    }
    else {
      for (size_t i = 0; i < MAX; ++i) {
        m.fn[i] = fn[i];
      }
    }

    // Menu rendered here
    dialog::render_menu(m.config, m.names);
    dialog::set_selectedIndex(m.config, 0);

    if (menus.size() > 1) {
      dialog::place_menu_relative(m.config, menus[menus.size() - 2].config);
      current_menu++;
    }
  }

  void draw() {
    for (auto &m: menus)
      dialog::draw(m.config);
  }


// TODO: Subclass and move under input/
/*
 * Menu -- Input handler
 */

  input::InputHandler menu;

  void menu_inc() {
    Menu &m = menus[current_menu];
    m.selected--;
    if (m.selected > m.names.size())
      m.selected = m.names.size() - 1;

    dialog::set_selectedIndex(m.config, m.selected);
  }

  void menu_dec() {
    Menu &m = menus[current_menu];
    m.selected++;
    if (m.selected >= m.names.size())
      m.selected = 0;

    dialog::set_selectedIndex(m.config, m.selected);
  }

  void menu_sel() {
    Menu &m = menus[current_menu];
    if (m.fn[m.selected])
      m.fn[m.selected]();
    //std::cout << m.names[m.selected] << '\n';
  }

  void menu_del() { puts("Menu del"); }

// TODO: Menu persistence to avoid re-creating new instances
  void menu_close() {

    // Leaving the root menu?
    if (current_menu == 0) {
      // Deallocate, set previous input handler
      current_menu = 0;
      menus.pop_back();
      input::SetPrevHandler();
    } else {
      menus.pop_back();
      current_menu--;
    }
  }

  void on_enable() {
    puts("Menu enabled");
    if (menus.size() == 0) {
      puts("No active_ menus");
      input::SetPrevHandler();
      return;
    }
  }

  void on_disable() {
    while (menus.size())
      menus.pop_back();
    current_menu = 0;
    puts("Menu closed");
  }

  void init_input_maps() {
    menu.fn[input::KEY::ON_ENABLE] = menu::on_enable;
    menu.fn[input::KEY::ON_DISABLE] = menu::on_disable;
    menu.fn[input::KEY::UP] = menu::menu_inc;
    menu.fn[input::KEY::DOWN] = menu::menu_dec;
    menu.fn[input::KEY::LEFT] = menu::menu_close;
    menu.fn[input::KEY::RIGHT] = menu::menu_sel;
    menu.fn[input::KEY::START] = menu::menu_close;

    constexpr uint32_t menu_mask = input::gen_mask(
        {
            input::KEY::ON_ENABLE,
            input::KEY::ON_DISABLE,
            input::KEY::UP,
            input::KEY::DOWN,
            input::KEY::LEFT,
            input::KEY::RIGHT,
            input::KEY::START
        }
    );
    menu.mask = menu_mask;
  }

  std::string lastSelectedTitle() {
    return menus[current_menu].names[current_menu];
  }

  size_t lastSelectedIndex() {
    return menus[current_menu].selected;
  }
}

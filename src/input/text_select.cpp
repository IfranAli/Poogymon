//
// Created by Ila Luvox on 16/11/21.
//

#include <cstdio>
#include "text_select.h"
#include "dbgmenu.h"

namespace text_select {
using namespace input;

InputHandler text_select;

void select_letter() {
  puts("Something should happen but nothing happened..");
}

void exit_dialog() {
  debug::open_menu();
}

void on_enable() {
  puts("text_select enabled");
}

void on_disable() {
  puts("text_select disabled");
}

void init() {
  text_select.fn[ON_ENABLE]  = on_enable;
  text_select.fn[ON_DISABLE] = on_disable;
  text_select.fn[A]          = select_letter; // Select letter.
  text_select.fn[B]          = select_letter; // Delete letter.
  text_select.fn[UP]         = select_letter; // Move cursor up.
  text_select.fn[DOWN]       = select_letter; // Move cursor down.
  text_select.fn[LEFT]       = select_letter; // Move cursor left.
  text_select.fn[RIGHT]      = select_letter; // Move cursor right.
  text_select.fn[START]      = select_letter; // Conform dialog.
  text_select.fn[SELECT]     = exit_dialog; // Cancel dialog.

  constexpr uint32_t text_select_mask = gen_mask(
      {ON_ENABLE, ON_DISABLE, A, B, UP, DOWN, LEFT, RIGHT, START, SELECT}
  );

  text_select.mask = text_select_mask;
}
} // namespace text_select

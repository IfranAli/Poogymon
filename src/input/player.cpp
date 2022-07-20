#include "player.h"
#include "dbgmenu.h"
#include "../drawable/CharacterPlayer.h"
#include "../Map.h"
#include "../Renderer.h"

namespace player {
  input::InputHandler ih_player;
  map::Map *map = nullptr;
  CharacterPlayer *character_player = nullptr;

  void vy_inc() {
    character_player->velocity_y_ = 1;
  }
  void vy_dec() {
    character_player->velocity_y_ = -1;
  }
  void vx_inc() {
    character_player->velocity_x_ = 1;
  }
  void vx_dec() {
    character_player->velocity_x_ = -1;
  }

  void v_clear() {
    character_player->velocity_y_ = 0;
    character_player->velocity_x_ = 0;
    character_player->is_running_ = 0;
  }

  void x_clear() { character_player->velocity_x_ = 0; }
  void y_clear() { character_player->velocity_y_ = 0; }

  void on_enable() {
    is_visible = true;
    v_clear();

    render::AddToRenderStack(character_player);
    puts("player enabled");
  }

  void on_disable() {
    is_visible = false;
    v_clear();

    render::RemoveFromRenderStack(character_player);
    puts("player disabled");
  }

  void open_menu() {
    debug::open_menu();
  }

  void set_running() { character_player->is_running_ = true; }
  void set_walking() { character_player->is_running_ = false; }

  void Init(map::Map *p_map, CharacterPlayer *p_player) {
    map = p_map;
    character_player = &*p_player;

    using namespace input;
    ih_player.fn[ON_ENABLE] = on_enable;
    ih_player.fn[ON_DISABLE] = on_disable;

    ih_player.fn[UP] = vy_dec;
    ih_player.fn[DOWN] = vy_inc;
    ih_player.fn[LEFT] = vx_dec;
    ih_player.fn[RIGHT] = vx_inc;

    ih_player.fn[UP_RELEASE] = y_clear;
    ih_player.fn[DOWN_RELEASE] = y_clear;
    ih_player.fn[LEFT_RELEASE] = x_clear;
    ih_player.fn[RIGHT_RELEASE] = x_clear;

    ih_player.fn[B] = set_running;
    ih_player.fn[B_RELEASE] = set_walking;

    ih_player.fn[START] = open_menu;

    constexpr uint32_t player_mask = gen_mask(
        {
            ON_ENABLE, ON_DISABLE,
            UP, DOWN, LEFT, RIGHT, START,
            UP_RELEASE, DOWN_RELEASE, LEFT_RELEASE, RIGHT_RELEASE,
            B, B_RELEASE
        }
    );

    ih_player.mask = player_mask;
  }
}

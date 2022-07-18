#include "Movement.h"
#include "../Renderer.h"
#include "../Map.h"

Movement::Movement() = default;
Movement::~Movement() = default;

//TODO: Adding a MovePseudo can help simplify code a bit

// TODO: Make free function
void Movement::MoveSmooth(int vx, int vy, bool move_camera) {
  if (moving) {
    return;
  }

  bool moving_x_axis = vx != 0;
  map::direction = (moving_x_axis ? vx : vy);
  pole = (moving_x_axis ? 1 : -1);
  map::pole = pole;

  if (move_camera) {
    axis = (moving_x_axis ? &map::active_map->x_ : &map::active_map->y_);
  } else {
    axis = (moving_x_axis ? &x : &y);
  }

  auto d = static_cast<float>(map::direction);
  target = *axis + (d);
  *axis += (d * speed) * sdl::deltatime;
  moving = true;
  map::moving = true;
}

void Movement::Tick() {
  //return moving && Step();
}

void Movement::Step() {
  auto d = static_cast<float>(map::direction);
  *axis += (d * speed) * sdl::deltatime;

  if ((d > 0.0 && *axis > target) || (d < 0.0 && *axis < target)) {
    *axis = target;
    moving = false;
    map::moving = false;
    map::pole = 0;
//    map::active_map->active->DrawMap(x, y);
  }
}

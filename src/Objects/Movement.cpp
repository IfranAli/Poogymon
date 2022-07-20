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
  direction = (moving_x_axis ? vx : vy);
  pole = (moving_x_axis ? 1 : -1);

  if (move_camera) {
    axis = (moving_x_axis ? &map::active_map->x_ : &map::active_map->y_);
  } else {
    axis = (moving_x_axis ? &x : &y);
  }

  auto d = static_cast<float>(direction);
  target = *axis + (d);
  *axis += (d * speed) * sdl::deltatime;
  moving = true;
}

void Movement::Tick() {
  //return moving && Step();
}

void Movement::Step() {
  auto d = static_cast<float>(direction);
  *axis += (d * speed) * sdl::deltatime;

  if ((d > 0.0 && *axis > target) || (d < 0.0 && *axis < target)) {
    *axis = target;
    moving = false;
  }
}

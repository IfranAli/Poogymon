#include "Movement.h"
#include "../Renderer.h"
#include "../World/Map.h"

Movement::Movement() = default;
Movement::~Movement() = default;

//TODO: Adding a MovePseudo can help simplify code a bit

// TODO: Make free function
void Movement::MoveSmooth(int vx, int vy, bool move_camera) {
  if (moving_) {
    return;
  }

  bool moving_x_axis = vx != 0;
  direction_ = (moving_x_axis ? vx : vy);
  pole_ = (moving_x_axis ? 1 : -1);

  if (move_camera) {
    axis_ = (moving_x_axis ? &map::active_map->x_ : &map::active_map->y_);
  } else {
    axis_ = (moving_x_axis ? &x_ : &y_);
  }

  auto direction = static_cast<float>(direction_);
  target_ = *axis_ + (direction);
  *axis_ += (direction * speed_) * sdl::deltatime;
  moving_ = true;
}

void Movement::Tick() {
  //return moving_ && Step();
}

void Movement::Step() {
  auto direction = static_cast<float>(direction_);
  *axis_ += (direction * speed_) * sdl::deltatime;

  if ((direction > 0.0 && *axis_ > target_) || (direction < 0.0 && *axis_ < target_)) {
    *axis_ = target_;
    moving_ = false;
  }
}

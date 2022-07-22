#ifndef CHARACTER_H
#define CHARACTER_H

#include "../Objects/Animation.h"
#include "../Objects/Movement.h"
#include "../interfaces/Drawable.h"

class Character : public Drawable {
 public:
  Character();
  ~Character();

  virtual void Tick();

  bool is_running_ = false;
  int velocity_x_ = 0;
  int velocity_y_ = 0;

  void ResetSpeed();

 protected:
  Animation animation_;
  Movement movement_;

 private:
  const float default_animation_speed_ = 0.20;
  const float default_movement_speed_ = 0.05F;
};
#endif

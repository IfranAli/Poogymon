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

  bool is_running {false};
        int mVelocityX;
        int mVelocityY;

		Animation animation;
		Movement movement;
};
#endif

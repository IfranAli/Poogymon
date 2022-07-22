//
// Created by Ila Luvox on 22/7/2022.
//

#ifndef GAME_SRC_RENDERMANAGER_H_
#define GAME_SRC_RENDERMANAGER_H_

#include <vector>
#include "interfaces/Drawable.h"

class RenderManager {
 public:
  RenderManager() = default;
  ~RenderManager() = default;

  Drawable *AddToRenderStack(Drawable *drawable);

  bool RemoveFromRenderStack(Drawable *p_drawable);
  void DrawStack();

 private:
  std::vector<Drawable *> render_stack_{};
};

#endif //GAME_SRC_RENDERMANAGER_H_

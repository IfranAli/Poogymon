//
// Created by Ila Luvox on 22/7/2022.
//

#include "RenderManager.h"

Drawable *RenderManager::AddToRenderStack(Drawable *drawable) {
  render_stack_.push_back(drawable);
  printf("Added to render stack");
  return drawable;
}

bool RenderManager::RemoveFromRenderStack(Drawable *p_drawable) {
  auto found = std::find(std::begin(render_stack_), std::end(render_stack_), p_drawable);

  if (*found) {
    render_stack_.erase(found);
  }

  return true;
}

void RenderManager::DrawStack() {
  for (Drawable *item: render_stack_) {
    item->Render();
  }
}

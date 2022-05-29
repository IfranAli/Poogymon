#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

namespace Texture {
struct Texture final {
public:
  Texture() = default;
  ~Texture();

  Texture(const Texture &t) noexcept;
  Texture(Texture &&) noexcept;
  Texture &operator=(Texture &&) noexcept;
  Texture &operator=(const Texture &t);

  SDL_Texture *mTexture{nullptr};
  int         mWidth{0};
  int         mHeight{0};
  std::string filename{};
};

void Render(Texture &t, SDL_Renderer &r, SDL_Rect *clip, SDL_Rect *scr);
void Render(Texture &t, SDL_Renderer &r, int x, int y, SDL_Rect *clip);
bool LoadFromFile(Texture &t, SDL_Renderer &r, const char *path);
void freeMem(Texture &t);
void MakeEmpty(Texture &t, SDL_Renderer &r, size_t w, size_t h);
void SetAsRenderTarget(const Texture &t, SDL_Renderer &r);
}

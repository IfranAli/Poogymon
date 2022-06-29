#include "Tileset.h"
#include "Renderer.h"

#include <utility>

namespace Texture {
  Tileset::Tileset() : info{0, 0, 0} {}

  Tileset::~Tileset() {}

  Tileset::Tileset(Tileset &&t) noexcept:
      texture(std::move(t.texture)),
      info(std::exchange(t.info, {0, 0, 0})) {}

  Tileset &Tileset::operator=(Tileset &&t) noexcept {
    texture = std::move(t.texture);
    info = std::exchange(t.info, {0, 0, 0});
    return *this;
  }

  Tileset::Tileset(const Tileset &t) noexcept {
    texture = t.texture;
    info = t.info;
  }

  int Tileset::GetTile(int x, int y) const {
    auto col = x / info.tile_width;
    auto row = y / info.tile_width;
    auto tile = (row * info.tile_per_row) + col;

    return tile;
  }

  bool LoadFromFile(SDL_Renderer &r, Texture &t, tileset_info &i, const char *path, int w, int h) {
    bool result = LoadFromFile(t, r, path);
    if (result)
      i = getTilesetInfo(t, w, h);
    return result;
  }

  bool LoadFromFile(SDL_Renderer &r, Tileset &t, const char *path, int w, int h) {
    return LoadFromFile(r, t.texture, t.info, path, w, h);
  }

  void Render(SDL_Renderer &r, const Texture &texture, const tileset_info &info, int i, int x, int y) {
    SDL_Rect src {
      x,
      y,
      info.tile_width,
      info.tile_height
    };

    getTile(texture, info, i, src);

    //Set rendering space and render to screen
    SDL_Rect dest = src;
    dest.x = x;
    dest.y = y;
    dest.w = info.tile_width;
    dest.h = info.tile_width;

    SDL_RenderCopy(&r, texture.mTexture, &src, &dest);
  }

  void Render(SDL_Renderer &r, const Tileset &tileset, int tile, int x, int y) {
    Render(r, tileset.texture, tileset.info, tile, x, y);
  }

  void getTile(const Texture &texture, const tileset_info &info, int i, SDL_Rect &clip) {
    int x = info.tile_width * (i % info.tile_per_row);
    int y = info.tile_height * (int) (i / info.tile_per_row);

    if (
        x > (texture.mWidth - info.tile_width) ||
            x < 0 ||
            y > (texture.mHeight - info.tile_height) ||
            y < 0) {
      printf("[T - %s] Invalid out of range gettile(%d)\n", texture.filename.c_str(), i);
      y = x = 0;
    }

    clip.x = x;
    clip.y = y;
  }

  tileset_info getTilesetInfo(const Texture &texture, int w, int h) {
    tileset_info info;
    info.tile_width = w;
    info.tile_height = h;
    info.tile_per_row = (texture.mWidth / info.tile_width);
    return info;
  }

} /* TM */

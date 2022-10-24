#include "Texture.h"

#include <SDL_ttf.h>

#include <utility>

namespace Texture {
/* *****
 * Texture implementation
 * *****/

// Move constructor
  Texture::Texture(Texture &&t) noexcept {
    mTexture = std::exchange(t.mTexture, nullptr);
    mWidth = std::exchange(t.mWidth, 0);
    mHeight = std::exchange(t.mHeight, 0);
    filename = t.filename;
  }

// Assignment
  Texture &Texture::operator=(Texture &&t) noexcept {
    freeMem(*this);
    mTexture = std::exchange(t.mTexture, nullptr);
    mWidth = std::exchange(t.mWidth, 0);
    mHeight = std::exchange(t.mHeight, 0);
    std::swap(filename, t.filename);
    return *this;
  }

// Copy Constructor
  Texture::Texture(const Texture &t) noexcept {
    mTexture = t.mTexture;
    mWidth = t.mWidth;
    mHeight = t.mHeight;
    filename = t.filename;
  }

  Texture::~Texture() {
    puts("Texture destroy");
    freeMem(*this);
  }

  Texture &Texture::operator=(const Texture &t) {
    mTexture = t.mTexture;
    mWidth = t.mWidth;
    mHeight = t.mHeight;
    filename = t.filename;
    return *this;
  }

  void Render(Texture &t, SDL_Renderer &r, SDL_Rect *clip, SDL_Rect *scr) {
    SDL_RenderCopy(&r, t.mTexture, scr, clip);
  }

// TODO: ???
  void Render(Texture &t, SDL_Renderer &r, int x, int y, SDL_Rect *src) {
//    SDL_Rect dest = {x_, y_, t.mWidth, t.mHeight};
//
//    if (src != NULL) {
////      dest.w = src->w;
////      dest.h = src->h;
//    }

    SDL_Rect dest = {0, 0, 100, 100};

    if (src != NULL) {
//      dest.w = src->w;
//      dest.h = src->h;
    }

    SDL_RenderCopy(&r, t.mTexture, src, &dest);
  }

  bool LoadFromFile(Texture &t, SDL_Renderer &r, const char *path) {
    freeMem(t);
    t.filename = std::string{path};
    printf("%s -- \n", t.filename.c_str());

    SDL_Texture *newTexture = nullptr;
    SDL_Surface *loadedSurface = IMG_Load(path);

    if (loadedSurface == nullptr) {
      printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
      //Color key image
      //SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

      //Create texture from surface pixels
      newTexture = SDL_CreateTextureFromSurface(&r, loadedSurface);
      if (newTexture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
      } else {
        t.mWidth = loadedSurface->w;
        t.mHeight = loadedSurface->h;
      }

      SDL_FreeSurface(loadedSurface);
    }

    t.mTexture = newTexture;
    return t.mTexture != NULL;
  }

  void freeMem(Texture &t) {
    if (!t.filename.empty()) {
      if (t.mTexture) {
        SDL_DestroyTexture(t.mTexture);
        t.mTexture = nullptr;
        t.mWidth = 0;
        t.mHeight = 0;
      }
    }
  }

  int GetWidth(const Texture &t) { return t.mWidth; }
  int GetHeight(const Texture &t) { return t.mHeight; }

  void MakeEmpty(Texture &t, SDL_Renderer &r, size_t w, size_t h) {
    freeMem(t);
    t.mWidth = w;
    t.mHeight = h;

    t.mTexture = SDL_CreateTexture(
        &r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

    if (!t.mTexture)
      puts("Could not create blank texture");
  }

  void SetAsRenderTarget(const Texture &t, SDL_Renderer &r) {
    if (SDL_SetRenderTarget(&r, t.mTexture) < 0) {
      printf("Texture: %s\n", SDL_GetError());
    }
  }
}

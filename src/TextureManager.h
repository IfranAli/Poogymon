//
// Created by Ila Luvox on 23/7/2022.
//

#ifndef GAME_SRC_TEXTUREMANAGER_H_
#define GAME_SRC_TEXTUREMANAGER_H_

#include <string>
#include <vector>
#include "Texture.h"

class TextureManager {
 public:
  TextureManager() = default;
  ~TextureManager() = default;

  Texture::Texture &GetTextureByName(std::string &);

 private:
  std::vector<std::string> getAvailableTexturesAsName();
};

#endif //GAME_SRC_TEXTUREMANAGER_H_

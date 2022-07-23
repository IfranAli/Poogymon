//
// Created by Ila Luvox on 23/7/2022.
//

#include "TextureManager.h"

namespace {
  Texture::Texture g_temp_texture = Texture::Texture();
}

Texture::Texture &TextureManager::GetTextureByName(std::string &) {
  // todo: implement.
  return g_temp_texture;
}
std::vector<std::string> TextureManager::getAvailableTexturesAsName() {
  return std::vector<std::string>();
}

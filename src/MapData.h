#pragma once
#include "Texture.h"
#include "Tileset.h"

namespace map_data {

class MapData;
struct Connections {
  MapData *Left = nullptr;
  MapData *Right = nullptr;
  MapData *Up = nullptr;
  MapData *Down = nullptr;
};

class MapData {
 public:
  void DrawMap();
  bool LoadMap(const std::string &p_file_name, const std::string &p_file_name_texture);
  void SaveMap(const std::string &filename);
  void SaveMap();
  void LoadMap();
  void PrintMap();

  int GetTile(int col, int row);
  void SetTile(int col, int row, int tile);
  void RenderTile(int col, int row);

  MapData(MapData &map_data) noexcept;
  MapData(MapData &&t) noexcept;
  MapData(std::string filename, std::string filename_texture);
  MapData &operator=(MapData &&) noexcept;
  MapData();
  ~MapData();

  int map_width{};
  int map_height{};
  std::string filename;
  unsigned int *p_map_data{nullptr};
  Connections connections;
  Texture::Texture map_texture;
  Texture::Tileset map_tileset;
 private:
  void FreeMemory();
};
}

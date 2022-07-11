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
    void DrawMap(int x = 0, int y = 0);
    bool LoadMap(const std::string &p_file_name, const std::string &p_file_name_texture);
    void SaveMap(const std::string &filename);
    void SaveMap();
    void LoadMap();
    void MakeEmpty(std::string &texture_file_name, const int tile[4]);
    void PrintMap();

    int GetTile(int col, int row);
    void SetTile(int col, int row, int tile);
    void RenderTile(int col, int row);

    MapData(MapData &map_data) noexcept;
    MapData(MapData &&t) noexcept;
    MapData(std::string filename, std::string filename_texture);
    explicit MapData(std::string filename_texture, int tile = 0);
    explicit MapData(std::string filename_texture, int tiles[4]);
    MapData();
    MapData &operator=(MapData &&) noexcept;
    ~MapData();

    int map_width{};
    int map_height{};
    std::string filename;
    unsigned int *p_map_data{nullptr};
    Connections connections;
    Texture::Texture map_texture;
    Texture::Tileset map_tileset;
   private:

    const int TILE_DIM = 32;
    void FreeMemory();
  };
}

#pragma once
#include "Texture.h"
#include "Tileset.h"
#include "Objects/FrameConfig.h"

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
    MapData();
    ~MapData();
    MapData(MapData &map_data) noexcept;
    MapData(MapData &&t) noexcept;

    MapData(std::string filename, std::string filename_texture);
    explicit MapData(std::string filename_texture, int tiles[4]);

    MapData &operator=(MapData &&) noexcept;

    void DrawMap();
    bool LoadMap(const std::string &p_file_name, const std::string &p_file_name_texture);
    void SaveMap(const std::string &filename);
    void SaveMap();
    void LoadMap();
    void MakeEmpty(std::string &texture_file_name, const int tile[4]);
    void PrintMap();

    int GetTile(int col, int row);
    void SetTile(int col, int row, int tile);
    void RenderTile(int col, int row);
    bool IsValidMap();

    [[nodiscard]] int GetMapWidth() const;
    [[nodiscard]] int GetMapHeight() const;

    std::string filename;
    Connections connections;
    Texture::Texture map_texture;
    Texture::Tileset map_tileset;
   private:

    int map_data_[config::MAP_ARRAY_SIZE];
    int map_height_ = 0;
    int map_width_ = 0;

    void FreeMemory();
  };
}

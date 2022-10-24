#pragma once
#include "../Texture.h"
#include "../Tileset.h"
#include "../Objects/FrameConfig.h"
#include <array>

namespace map_data {

  class MapData;
  struct Connections {
    MapData *Left = nullptr;
    MapData *Right = nullptr;
    MapData *Up = nullptr;
    MapData *Down = nullptr;
  };

  struct TilePattern {
    TilePattern(int tile) {
      tiles.fill(tile);
    }

    TilePattern(int tile_1, int tile_2, int tile_3, int tile_4) {
      tiles[0] = tile_1;
      tiles[1] = tile_2;
      tiles[2] = tile_3;
      tiles[3] = tile_4;
    }

    std::array<int, 4> &GetTilesArray() {
      return tiles;
    }

   private:
    std::array<int, 4> tiles{0, 0, 0, 0};
  };

  class MapData {
   public:
    MapData();
    ~MapData();
    MapData(MapData &map_data) noexcept;
    MapData(MapData &&t) noexcept;

    MapData(std::string filename, std::string filename_texture);
    explicit MapData(std::string filename_texture, TilePattern &tile_pattern);

    MapData &operator=(MapData &&) noexcept;

    void DrawMap();
    bool LoadMap(const std::string &p_file_name, const std::string &p_file_name_texture);
    void SaveMap(const std::string &filename);
    void SaveMap();
    void LoadMap();
    void MakeEmpty(std::string &texture_file_name, TilePattern &tile_pattern);
    void PrintMap();

    int GetTile(int col, int row);
    void SetTile(int col, int row, int tile);
    void RenderTile(int col, int row);
    bool IsValidMap() const;

    [[nodiscard]] int GetMapWidth() const;
    [[nodiscard]] int GetMapHeight() const;

    std::string filename_;
    Connections connections_;
    Texture::Texture map_texture_;
    Texture::Tileset map_tileset_;
   private:

    std::array<int, config::MAP_ARRAY_SIZE> map_data_;
    int map_height_ = 0;
    int map_width_ = 0;

    void FreeMemory();
  };
}

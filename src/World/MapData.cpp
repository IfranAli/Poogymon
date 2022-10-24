#include "MapData.h"
#include "../Renderer.h"
#include "Map.h"

#include <fstream>
#include <ostream>
#include <string>
#include <iostream>
#include <utility>

namespace map_data {
  MapData::MapData() {
    connections_ = {nullptr, nullptr, nullptr, nullptr};
  }

  MapData::MapData(std::string texture_file_name, TilePattern &tile_pattern) {
    connections_ = {nullptr, nullptr, nullptr, nullptr};
    MakeEmpty(texture_file_name, tile_pattern);

    PrintMap();
    DrawMap();
  }

  MapData::MapData(std::string filename, std::string filename_texture) {
    connections_ = {nullptr, nullptr, nullptr, nullptr};
    LoadMap(filename, filename_texture);
  }

  MapData::MapData(MapData &t) noexcept {
    filename_ = std::exchange(t.filename_, {});
    map_width_ = std::exchange(t.map_width_, 0);
    map_height_ = std::exchange(t.map_height_, 0);
    std::swap(connections_, t.connections_);
    std::swap(map_tileset_, t.map_tileset_);
    std::swap(map_texture_, t.map_texture_);
  }

  MapData &MapData::operator=(MapData &&t) noexcept {
    filename_ = std::exchange(t.filename_, {});

    for (auto i{0}; i < t.map_width_ * t.map_height_; ++i) {
      map_data_[i] = t.map_data_[i];
    }
    map_width_ = std::exchange(t.map_width_, 0);
    map_height_ = std::exchange(t.map_height_, 0);
    std::swap(connections_, t.connections_);
    std::swap(map_tileset_, t.map_tileset_);
    std::swap(map_texture_, t.map_texture_);
    return *this;
  }

// Move constructor
  MapData::MapData(MapData &&t) noexcept {
    filename_ = std::exchange(t.filename_, {});
    map_width_ = std::exchange(t.map_width_, 0);
    map_height_ = std::exchange(t.map_height_, 0);
//  connections_ = std::exchange(t.connections_, {});
    std::swap(connections_, t.connections_);
  }

  MapData::~MapData() {
    std::printf("map_data Dtor - %s \n", filename_.c_str());
    FreeMemory();
    std::printf("map_data Dtor - OK \n");
  }

  void MapData::DrawMap() {
    FrameConfig frame_config;
    auto dim = frame_config.GetTileDimension();

    SDL_Rect grass_tile{0, 0, dim, dim};
    SDL_Rect src_tile{1, 0, dim, dim};
    SDL_Rect dest_tile = src_tile;
    auto tileset_info = Texture::getTilesetInfo(&map_tileset_.texture, dim, dim);

    // Set render target_ to texture
    SDL_SetRenderTarget(sdl::renderer, map_texture_.mTexture);

    for (int i = 0; i < frame_config.GetRows(); ++i) {
      for (int j = 0; j < frame_config.GetCols(); ++j) {
        auto x_dim = dim * j;
        auto y_dim = dim * i;
        auto tile = GetTile(j, i);

        Texture::getTile(map_tileset_.texture, tileset_info, tile, src_tile);
        dest_tile.x = x_dim;
        dest_tile.y = y_dim;

        SDL_RenderCopy(
            sdl::renderer,
            map_tileset_.texture.mTexture,
            &grass_tile,
            &dest_tile
        );
        SDL_RenderCopy(
            sdl::renderer,
            map_tileset_.texture.mTexture,
            &src_tile,
            &dest_tile
        );
      }
    }

    SDL_SetRenderTarget(sdl::renderer, NULL);
  }

  bool MapData::LoadMap(const std::string &p_file_name, const std::string &p_file_name_texture) {
    FreeMemory();
    filename_ = p_file_name;

    std::ifstream ifs(filename_);
    ifs >> map_width_ >> map_height_;

    size_t array_size = map_width_ * map_height_;
    if (array_size == 0) {
      puts("map dimensions must be non-zeo");
      return false;
    }

    printf("Creating Map: %s\n", filename_.c_str());

    for (unsigned i = 0; i < array_size; ++i) {
      unsigned int tile = 0;
      ifs >> tile;
      map_data_[i] = tile;
    }

    ifs.close();

    unsigned int width = config::TILE_DIMENSIONS * map_width_;
    unsigned int height = config::TILE_DIMENSIONS * map_height_;

    Texture::MakeEmpty(map_texture_, *sdl::renderer, width, height);

    std::string file_path = "Resource/" + p_file_name_texture;
    Texture::LoadFromFile(*sdl::renderer,
                          map_tileset_,
                          file_path.c_str(),
                          config::TILE_DIMENSIONS,
                          config::TILE_DIMENSIONS);

//    SaveMap();
    PrintMap();
    DrawMap();

    printf("Loaded map_data: %s\n", filename_.c_str());

    return true;
  }

  void MapData::SaveMap() {
    if (!filename_.empty()) {
      SaveMap(filename_);
    }
  }

  void MapData::LoadMap() {
    if (!filename_.empty() && !map_tileset_.texture.filename.empty()) {
      LoadMap(filename_, map_tileset_.texture.filename);
    }
  }

  void MapData::MakeEmpty(std::string &texture_file_name, TilePattern &tile_pattern) {
    FreeMemory();

    map_width_ = config::TILE_PER_COLUMN;
    map_height_ = config::TILE_PER_ROW;

    std::array<int, 4> tiles = tile_pattern.GetTilesArray();
    auto tile = 0;

    for (auto y = 0; y < map_height_; ++y) {
      tile = (y % 2) ? 0 : 2;

      for (auto x = 0; x < map_width_; ++x) {
        map_data_[(y * map_width_) + x] = tiles.at(tile + (x % 2));
      }
    }

    int width = config::TILE_DIMENSIONS * map_width_;
    int height = config::TILE_DIMENSIONS * map_height_;

    Texture::MakeEmpty(map_texture_, *sdl::renderer, width, height);

    std::string file_path = "Resource/" + texture_file_name;
    Texture::LoadFromFile(*sdl::renderer,
                          map_tileset_,
                          file_path.c_str(),
                          config::TILE_DIMENSIONS,
                          config::TILE_DIMENSIONS);

    printf("Created map data!");
  }

  void MapData::SaveMap(const std::string &filename) {
    if (map_width_ == 0 || map_height_ == 0) {
      printf("Can not save matrix of size 0\n");
      return;
    }
//      auto array_size = map_width_ * map_height_;

//      for (int i = 0; i < array_size; ++i) {
//        unsigned int tile = p_map_data[i];
//        std::cout << tile << ' ';

    this->filename_ = filename;
    std::ofstream ofs(filename);
//
    ofs << map_width_ << ' ' << map_height_ << ' ';

    size_t map_array_size = map_width_ * map_height_;

    for (size_t i = 0; i < map_array_size; ++i) {
//        unsigned int tile = map_data_[i];
//        std::cout << tile << ' ';
//        ofs << tile << ' ';
    }

    for (unsigned i = 0; i < map_array_size; ++i) {
//        unsigned int tile = map_data_[i];
//        ofs << tile << ' ';
//        std::cout << tile << ' ';
//        std::cout << tile << ' ';
    }

//      ofs.close();
    printf("Saved Matrix: %s\n", filename.c_str());
  }

  void MapData::PrintMap() {
    for (int j = 0; j < map_height_; ++j) {
      for (int i = 0; i < map_width_; ++i) {
        printf("%2d ", GetTile(i, j));
      }
      printf("\n");
    }
  }

  void MapData::FreeMemory() {
  }

  int MapData::GetTile(int col, int row) {
    if (row < 0) {
      printf("Get tile has negative values\n");
      return -1;
    }

    if (col < 0) {
      if (map::active_map->HasConnection(map::LEFT_CONNECTION)) {
        auto &connection = map::active_map->GetConnection(map::LEFT_CONNECTION);
        col = connection.map_width_ + col;
        return connection.GetTile(col, row);
      }
    }

    if (row >= map_height_) {
      return ((connections_.Up) ? connections_.Up->GetTile(col, row % map_height_) : -1);
    }
    if (col >= map_width_) {
      return ((connections_.Right) ? connections_.Right->GetTile(col % map_width_, row) : -1);
    }

    return map_data_[row * map_width_ + col];
  }

  void MapData::SetTile(int col, int row, int t) {
    if (row < 0) {
      printf("Set Tile Negative row or col\n");
      return;
    }

    if (col < 0) {
      if (map::active_map->HasConnection(map::LEFT_CONNECTION)) {
        auto &connection = map::active_map->GetConnection(map::LEFT_CONNECTION);
        col = connection.map_width_ + col;
        return connection.SetTile(col, row, t);
      }
    }

    if (row >= map_height_) {
      if (connections_.Up) {
        connections_.Up->SetTile(col, row % map_height_, t);
      }
      return;
    }
    if (col >= map_width_) {
      if (connections_.Right) {
        connections_.Right->SetTile(col % map_width_, row, t);
      }
      return;
    }

    printf("MAP STORED TILE: %d\n", t);
    map_data_[row * map_width_ + col] = t;
    RenderTile(col, row);
  }

  void MapData::RenderTile(int col, int row) {
    int tile_x = col * config::TILE_DIMENSIONS;
    int tile_y = row * config::TILE_DIMENSIONS;

    Texture::SetAsRenderTarget(map_texture_, *sdl::renderer);

    //int tile = GetTile(col, row);
    //printf("PLACING TILE %d\n", tile);
    //map_tileset_.Render(0, NULL, tile_x, tile_y); // Grass.
    //map_tileset_.Render(GetTile(col, row), NULL, tile_x, tile_y);

    Texture::Render(*sdl::renderer, map_tileset_, 0, tile_x, tile_y);
    Texture::Render(*sdl::renderer, map_tileset_, GetTile(col, row), tile_x, tile_y);

    SDL_SetRenderTarget(sdl::renderer, NULL);
  }
  bool MapData::IsValidMap() const {
    return (map_width_ != 0) && (map_height_ != 0);
  }
  int MapData::GetMapWidth() const {
    return map_width_;
  }
  int MapData::GetMapHeight() const {
    return map_height_;
  }
}
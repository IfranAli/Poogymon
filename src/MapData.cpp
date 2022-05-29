#include "MapData.h"
#include "Renderer.h"
#include "Map.h"

#include <fstream>
#include <ostream>
#include <string>
#include <iostream>
#include <utility>

namespace map_data {
using namespace config;

MapData::MapData() {
  connections = {nullptr, nullptr, nullptr, nullptr};
  p_map_data = nullptr;
}

MapData::MapData(std::string filename, std::string filename_texture) {
  connections = {nullptr, nullptr, nullptr, nullptr};
  p_map_data = nullptr;
  LoadMap(filename, filename_texture);
}

MapData::MapData(MapData &t) noexcept {
  p_map_data = std::exchange(t.p_map_data, nullptr);
  filename = std::exchange(t.filename, {});
  map_width = std::exchange(t.map_width, 0);
  map_height = std::exchange(t.map_height, 0);
  std::swap(connections, t.connections);
  std::swap(map_tileset, t.map_tileset);
  std::swap(map_texture, t.map_texture);
}

MapData &MapData::operator=(MapData &&t) noexcept {
  p_map_data = std::exchange(t.p_map_data, nullptr);
  filename = std::exchange(t.filename, {});
  map_width = std::exchange(t.map_width, 0);
  map_height = std::exchange(t.map_height, 0);
  std::swap(connections, t.connections);
  std::swap(map_tileset, t.map_tileset);
  std::swap(map_texture, t.map_texture);
  return *this;
}

// Move constructor
MapData::MapData(MapData &&t) noexcept {
  p_map_data = std::exchange(t.p_map_data, nullptr);
  filename = std::exchange(t.filename, {});
  map_width = std::exchange(t.map_width, 0);
  map_height = std::exchange(t.map_height, 0);
//  connections = std::exchange(t.connections, {});
  std::swap(connections, t.connections);
}

MapData::~MapData() {
  std::printf("map_data Dtor - %s \n", filename.c_str());
  FreeMemory();
  std::printf("map_data Dtor - OK \n");
}

void MapData::DrawMap() {
  // Set render target to texture
  Texture::SetAsRenderTarget(map_texture, *sdl::renderer);

  // Render map with tileset.
  int x = 0;
  int y = 0;

  int c = 0;
  for (int i = 0; i < map_height; ++i) {
    for (int j = 0; j < map_width; ++j) {
      // Calcualte x, y render pos for dest texture
      x = TILE_DIM * j;
      y = TILE_DIM * i;

      // Render tile at pos x, y
      //map_tileset.Render(0, NULL, x, y); // Grass.
      //map_tileset.Render(GetTile(j, i), NULL, x, y);
      Texture::Render(*sdl::renderer, map_tileset, 0, x, y);
      Texture::Render(*sdl::renderer, map_tileset, GetTile(j, i), x, y);
      c++;
    }
  }

  std::printf("%s rendered %d tiles\n", filename.c_str(), c);

  // Reset render target to screen
  SDL_SetRenderTarget(sdl::renderer, NULL);
}

bool MapData::LoadMap(const std::string &p_file_name, const std::string &p_file_name_texture) {
  FreeMemory();
  filename = p_file_name;

  std::ifstream ifs(filename);
  ifs >> map_width >> map_height;

  size_t array_size = map_width * map_height;
  if (array_size == 0) {
    puts("map dimensions must be non-zeo");
    return false;
  }

  printf("Creating Map: %s\n", filename.c_str());
  p_map_data = new unsigned int[array_size];

  for (unsigned i = 0; i < array_size; ++i) {
    unsigned int tile = 0;
    ifs >> tile;
    std::cout << tile << ' ';

//    ifs >> p_map_data[i];
    p_map_data[i] = tile;
  }

  ifs.close();

  size_t width = TILE_DIM * map_width;
  size_t height = TILE_DIM * map_height;

  Texture::MakeEmpty(map_texture, *sdl::renderer, width, height);

  std::string file_path = "Resource/" + p_file_name_texture;
  Texture::LoadFromFile(*sdl::renderer, map_tileset, file_path.c_str(), TILE_DIM, TILE_DIM);

  DrawMap();

  printf("Loaded map_data: %s\n", filename.c_str());

  return true;
}

void MapData::SaveMap() {
  if (!filename.empty()) {
    SaveMap(filename);
  }
}

void MapData::LoadMap() {
  if (!filename.empty() && !map_tileset.texture.filename.empty()) {
    LoadMap(filename, map_tileset.texture.filename);
  }
}

void MapData::SaveMap(const std::string &filename) {
  if (p_map_data == nullptr) {
    printf("Matrix is NULL, can not save.\n");
    return;
  } else if (map_width == 0 || map_height == 0) {
    printf("Can not save matrix of size 0\n");
    return;
  } else {

    auto array_size = map_width * map_height;

    for (int i = 0; i < array_size; ++i) {
      unsigned int tile = p_map_data[i];
      std::cout << tile << ' ';
    }

    this->filename = filename;
    std::ofstream ofs(filename);

    ofs << map_width << ' ' << map_height << ' ';

    size_t map_array_size = map_width * map_height;

    for (size_t i = 0; i < map_array_size; ++i) {
      unsigned int tile = p_map_data[i];
      ofs << tile << ' ';
    }

    ofs.close();
    printf("Saved Matrix: %s\n", filename.c_str());
  }
}

void MapData::PrintMap() {
  if (p_map_data == nullptr) {
    std::printf("map data is nullptr, can not print.\n");
    return;
  }

  for (int j = 0; j < map_height; ++j) {
    for (int i = 0; i < map_width; ++i) {
      //printf("%2d ", p_map_data[j * map_width + i]);
      printf("%2d ", GetTile(i, j));
    }
    printf("\n");
  }
}

void MapData::FreeMemory() {
  if (p_map_data != nullptr) {
    // TODO: still crashes
    delete[] p_map_data;
    p_map_data = nullptr;
    map_height = 0;
    map_width = 0;
    filename = "";
  }
}


int MapData::GetTile(int col, int row) {
  if (row < 0) {
    printf("Get tile has negative values\n");
    return -1;
  }

  if (col < 0) {
    if (map::active_map->HasConnection(map::LEFT_CONNECTION)) {
      auto &connection = map::active_map->GetConnection(map::LEFT_CONNECTION);
      col = connection.map_width + col;
      return connection.GetTile(col, row);
    }
  }

  if (row >= map_height) {
    return ((connections.Up) ? connections.Up->GetTile(col, row % map_height) : -1);
  }
  if (col >= map_width) {
    return ((connections.Right) ? connections.Right->GetTile(col % map_width, row) : -1);
  }

  return p_map_data[row * map_width + col];
}

void MapData::SetTile(int col, int row, int t) {
  if (row < 0) {
    printf("Set Tile Negative row or col\n");
    return;
  }

  if (col < 0) {
    if (map::active_map->HasConnection(map::LEFT_CONNECTION)) {
      auto &connection = map::active_map->GetConnection(map::LEFT_CONNECTION);
      col = connection.map_width + col;
      return connection.SetTile(col, row, t);
    }
  }

  if (row >= map_height) {
    if (connections.Up) {
      connections.Up->SetTile(col, row % map_height, t);
    }
    return;
  }
  if (col >= map_width) {
    if (connections.Right) {
      connections.Right->SetTile(col % map_width, row, t);
    }
    return;
  }

  printf("MAP STORED TILE: %d\n", t);
  p_map_data[row * map_width + col] = t;
  RenderTile(col, row);
}

void MapData::RenderTile(int col, int row) {
  int x = col * TILE_DIM;
  int y = row * TILE_DIM;

  Texture::SetAsRenderTarget(map_texture, *sdl::renderer);

  //int tile = GetTile(col, row);
  //printf("PLACING TILE %d\n", tile);
  //map_tileset.Render(0, NULL, x, y); // Grass.
  //map_tileset.Render(GetTile(col, row), NULL, x, y);

  Texture::Render(*sdl::renderer, map_tileset, 0, x, y);
  Texture::Render(*sdl::renderer, map_tileset, GetTile(col, row), x, y);

  SDL_SetRenderTarget(sdl::renderer, NULL);
}

}
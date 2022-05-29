#include <iostream>
#include "../src/MapData.h"
#include "Random/ClassA.h"
int main() {
	ClassA a;

//	map_data::map_data map("Map01.map");

  a.map.PrintMap();

	int x, y, tile;	
	tile = x = y  = 0;

	while (tile != -1 && x != -1 && y != -1) {
		std::cout << "usage: x y tile\n";

		std::cin >> x >> y >> tile;

		a.map.SetTile(x, y, tile);

      a.map.PrintMap();
	}

	std::cout << "Hello world\n";
}

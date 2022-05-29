#include "../src/Matrix.h"
#include <iostream>

static const char* FILENAME = "Map01.map";

void TestSaveLoad(const char* filename) {
	Matrix m;
	m.Init(10,10);
	m.AddTestData(10);
	m.PrintMatrix();
	m.SaveMatrix(filename);

	Matrix n;
	n.LoadMatrix(filename);
	n.PrintMatrix();
}

int main () {
    //TestSaveLoad("1.mat");
	Matrix n;
    n.SetScreen(15,10);
	n.LoadMatrix(FILENAME);

	char i = 'w';
	while (i!= 'q') {
		std::cin >> i;
		switch (i) {
		case 'w':
			n.MoveY(-1);
			std::cout << "up\n";
				break;
		case 's':
			n.MoveY(1);
			std::cout << "down\n";
				break;
		case 'a':
			n.MoveX(-1);
			std::cout << "left\n";
				break;
		case 'd':
			n.MoveX(1);
			std::cout << "right\n";
				break;
		}
		n.PrintRelative();
	}
	return 0;
}

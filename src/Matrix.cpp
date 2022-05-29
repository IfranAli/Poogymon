#include "Matrix.h"
#include <iostream>
#include <fstream>
#include <sstream>

Matrix::Matrix () {
	Matrix::pMatrix = NULL;
	Matrix::mWidth = 0;
	Matrix::mHeight = 0;

	Matrix::mScreenWidth = 15;
	Matrix::mScreenHeight = 10;
	Matrix::mX = 0;
	Matrix::mY = 0;

}

void Matrix::SetScreen(int w, int h) {
    printf("Matrix SetScreen: w:%d, h:%d\n", w, h);
    mScreenWidth = w;
    mScreenHeight = h;
}
int Matrix::GetScreenHeight() {return mScreenHeight; }
int Matrix::GetScreenWidth() { return mScreenWidth; }
bool Matrix::SetScreenHeight(int s) {return 0; }
bool Matrix::SetScreenWidth(int s) { return 0; }

Matrix::~Matrix () {
	freeMemory();
	printf("Matrix Dtor\n");
}

void Matrix::freeMemory() {
	if (pMatrix != NULL) {
		for (int i=0; i<mHeight; ++i)
			delete[] pMatrix[i];
		delete[] pMatrix;
		pMatrix = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Matrix::Init( int width, int height ) {
	std::printf("Matrix Init: width:%d, height:%d\n", width, height);
	Matrix::mHeight = height;
	Matrix::mWidth = width;

	pMatrix = new int*[mHeight];
	for (int i=0; i<mHeight; ++i) {
		pMatrix[i] = new int[mWidth];
	}
}

void Matrix::PrintMatrix() {
	if (pMatrix == NULL) {
		std::printf("Matrix is NULL, can not print.\n");
	} else {
		for (int i=0; i<mHeight; ++i) {
			for (int j=0; j<mWidth; ++j) {
				printf("%2d ", pMatrix[i][j]);
			}
			printf("\n");
		}
	}
}

bool Matrix::isMaxX() {
    //printf("mX %d > mWidth %d - mScreenWidth %d = %d\n", mX, mWidth, mScreenWidth, mWidth - mScreenWidth);
	return (mX == (mWidth - mScreenWidth));
}
bool Matrix::isMaxY() {
	return (mY == (mHeight- mScreenHeight));
}
bool Matrix::MoveX(int x) {
	mX+= x;
	if ( (mX > (mWidth - mScreenWidth)) || (mX < 0)) {
		printf("Reached max +-X.\n");
		mX-= x;
        return false;
	}
	printf("x: %d, y: %d\n", mX, mY);
    return true;
}
bool Matrix::MoveY(int y) {
	mY+= y;
	if ( mY > (mHeight - mScreenHeight) || (mY < 0)) {
		printf("Reached max +-Y.\n");
		mY-= y;
        return false;
	}
	printf("x: %d, y: %d\n", mX, mY);
    return true;
}
bool Matrix::Move(int x, int y) {
	mX+= x;
	mY+= y;
	if ( (mX > (mWidth - mScreenWidth)) || (mX < 0)) {
		printf("Reached max +-X.\n");
		mX-= x;
        return false;
	}
	if ( mY > (mHeight - mScreenHeight) || (mY < 0)) {
		printf("Reached max +-Y.\n");
		mY-= y;
        return false;
	}
	printf("x: %d, y: %d\n", mX, mY);
    return true;
}

bool Matrix::SetXY(int x, int y) {
	if ((x <= (mWidth - mScreenWidth) && (x > 0)) &&
		(y <= (mHeight - mScreenHeight) && y > 0)) {
		mX = x;
		mY = y;
		return true;
	}
	return false;
}

void Matrix::PrintRelative() {
	for (int i=0; i<mScreenHeight; ++i) {
		for (int j=0; j<mScreenWidth; ++j) {
			printf("%3d ", pMatrix[i+mY][j+mX]);
		}
		printf("\n");
	}
	printf("\n");
}

void Matrix::AddTestData(int seed) {
	if (pMatrix == NULL) {
		std::printf("Matrix is NULL, can not add test data.\n");
	} else {
		for (int i=0; i<mHeight; ++i) {
			for (int j=0; j<mWidth; ++j) {
				pMatrix[i][j] = seed+j+i;
			}
		}
	}
}

void Matrix::SaveMatrix(const char* pFilename ) {
	if (pMatrix == NULL) {
		std::printf("Matrix is NULL, can not save.\n");
	} else if (mWidth == 0 && mHeight == 0) {
		std::printf("Can not save matrix of size 0");
	} else {
		std::ofstream ofs(pFilename);
		ofs << mWidth << ' ' << mHeight << ' ';

		for (int i=0; i<mHeight; ++i) {
			for (int j=0; j<mWidth; ++j) {
				ofs << pMatrix[i][j] << ' ';
			}
		}
		ofs.close();
		printf("Saved Matrix: %s\n", pFilename);
	}
}

void Matrix::LoadMatrix(const char* pFilename) {
	freeMemory();
	std::ifstream ifs(pFilename);
	ifs >> mWidth >> mHeight;

	Init(mWidth, mHeight);

	for (int i=0; i<mHeight; ++i){
		for (int j=0; j<mWidth; ++j) {
			ifs >> pMatrix[i][j];
		}
	}
	ifs.close();
	printf("Loaded Matrix: %s\n", pFilename);
}

int Matrix::GetX() {
    return mX;
}
int Matrix::GetY() {
    return mY;
}
int Matrix::GetWidth() {
    return Matrix::mWidth;
}
int Matrix::GetHeight() {
    return Matrix::mHeight;
}
int Matrix::GetTile(int row, int col) {
	if (row < 0 || col < 0 || row >= mHeight || col >= mWidth) {
		//std::cout << "GET out of range\n";
		return -1;
	}
    return pMatrix[row][col];
}
void Matrix::SetTile(int row, int col, int t) {
	if (row >= mHeight || col >= mWidth) 
	{
		std::cout << "Overflow\n";
		return;
	}
    pMatrix[row][col] = t;
}

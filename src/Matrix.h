#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
	public:
		Matrix ();
		virtual ~Matrix ();

		// Init sould be private.
		void Init(int width, int height);

		void PrintMatrix();
		void AddTestData(int seed);

		void SaveMatrix(const char*);
		void LoadMatrix(const char*);

		// Rename methods.
		void PrintRelative();
        bool SetXY(int x, int y);
        bool Move(int x, int y);
        bool MoveX(int x);
        bool MoveY(int y);
        bool isMaxX();
        bool isMaxY();

        int GetHeight();
        int GetWidth();

        int GetX();
        int GetY();

        int GetTile(int row, int col);
        void SetTile(int row, int col, int t);

        void SetScreen(int x, int y);
		int GetScreenHeight();
		int GetScreenWidth();
		bool SetScreenHeight(int);
		bool SetScreenWidth(int);
	
		int mWidth;
		int mHeight;
	protected:
		int** pMatrix;
		int mX;
		int mY;

		// Move out of here.
		int mScreenWidth;
		int mScreenHeight;

	private:
		void freeMemory();
};

#endif

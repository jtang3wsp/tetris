#pragma once
#include <utility>
#include "grid_data.h"

class Tetromino {
private:
	int shape;
	int rotation;
	std::pair<int, int> minos[4];
public:
	Tetromino(int shape, int rotation);
	/*
	int getShape();
	int getRotation();
	std::pair<int, int>* getMinos();
	*/
	void setPiecePos(int grid[][WINDOW_COLUMNS], int position);
	bool canMove(int grid[][WINDOW_COLUMNS]);
	void moveDown(int grid[][WINDOW_COLUMNS], bool canMove);
	bool moveLeft(int grid[][WINDOW_COLUMNS]);
	bool moveRight(int grid[][WINDOW_COLUMNS]);
	std::pair<int, int>* updateGhost(int grid[][WINDOW_COLUMNS]);
	void hardDrop(int grid[][WINDOW_COLUMNS]);
	bool hold(int grid[][WINDOW_COLUMNS], Tetromino &holdPiece);
	bool rotateRight(int grid[][WINDOW_COLUMNS]);
	bool rotateLeft(int grid[][WINDOW_COLUMNS]);
	int mod(int x, int m);
};


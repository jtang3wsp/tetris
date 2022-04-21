#include "Tetromino.h"

// CONSTRUCTOR
// Input:
//		shape: integer from 1 to 7 that determines what shape the Tetromino will be
//		rotation: integer from 0 to 3 that determines the orientation of the piece
Tetromino::Tetromino(int shape, int rotation) {
	this->shape = shape;
	this->rotation = rotation;
	
	// minos are the relative positions of each mino, with rotation point as the origin (0, 0)
	switch (shape) {
	case 1: // I piece
		minos[0].first = 0;
		minos[0].second = 0;
		minos[1].first = 0;
		minos[1].second = -1;
		minos[2].first = 0;
		minos[2].second = 1;
		minos[3].first = 0;
		minos[3].second = 2;
		break;
	case 2: // O piece
		minos[0].first = 0;
		minos[0].second = 0;
		minos[1].first = 0;
		minos[1].second = 1;
		minos[2].first = -1;
		minos[2].second = 1;
		minos[3].first = -1;
		minos[3].second = 0;
		break;
	case 3: // T piece
		minos[0].first = 0;
		minos[0].second = 0;
		minos[1].first = 0;
		minos[1].second = -1;
		minos[2].first = 0;
		minos[2].second = 1;
		minos[3].first = -1;
		minos[3].second = 0;
		break;
	case 4: // S piece
		minos[0].first = 0;
		minos[0].second = 0;
		minos[1].first = 0;
		minos[1].second = -1;
		minos[2].first = -1;
		minos[2].second = 0;
		minos[3].first = -1;
		minos[3].second = 1;
		break;
	case 5: // Z piece
		minos[0].first = 0;
		minos[0].second = 0;
		minos[1].first = 0;
		minos[1].second = 1;
		minos[2].first = -1;
		minos[2].second = 0;
		minos[3].first = -1;
		minos[3].second = -1;
		break;
	case 6: // L piece
		minos[0].first = 0;
		minos[0].second = 0;
		minos[1].first = 0;
		minos[1].second = -1;
		minos[2].first = 0;
		minos[2].second = 1;
		minos[3].first = -1;
		minos[3].second = 1;
		break;
	case 7: // J piece
		minos[0].first = 0;
		minos[0].second = 0;
		minos[1].first = 0;
		minos[1].second = 1;
		minos[2].first = 0;
		minos[2].second = -1;
		minos[3].first = -1;
		minos[3].second = -1;
		break;
	}
}

/*
int Tetromino::getShape() {
	return this->shape;
}

int Tetromino::getRotation() {
	return this->rotation;
}

std::pair<int, int> * Tetromino::getMinos() {
	return this->minos;
}
*/

// Updates the grid with the Tetromino's position
// Input:
//		grid: The grid in which the entire game takes place; will be passed to nearly every method
//		position: integer from 0 to 6 indicating which position the Tetromino should be at
void Tetromino::setPiecePos(int grid[][WINDOW_COLUMNS], int position) {
	// offset variables to convert global coordinates to relative coordinates
	int global_offset_row = minos[0].first;
	int global_offset_column = minos[0].second;

	switch (position) { // converts relative coordinates to new global coordinates
	case 0: // top of grid, above the board (spawn)
		for (int i = 0; i < 4; i++) {
			minos[i].first = minos[i].first - global_offset_row + 1;
			minos[i].second = minos[i].second - global_offset_column + 10;
		}
		break;
	case 1: // queue 1 (top/front of queue; next piece)
		for (int i = 0; i < 4; i++) {
			minos[i].first = minos[i].first - global_offset_row + 3;
			minos[i].second = minos[i].second - global_offset_column + 19;
		}
		break;
	case 2: // queue 2
		for (int i = 0; i < 4; i++) {
			minos[i].first = minos[i].first - global_offset_row + 6;
			minos[i].second = minos[i].second - global_offset_column + 19;
		}
		break;
	case 3: // queue 3
		for (int i = 0; i < 4; i++) {
			minos[i].first = minos[i].first - global_offset_row + 9;
			minos[i].second = minos[i].second - global_offset_column + 19;
		}
		break;
	case 4: // queue 4
		for (int i = 0; i < 4; i++) {
			minos[i].first = minos[i].first - global_offset_row + 12;
			minos[i].second = minos[i].second - global_offset_column + 19;
		}
		break;
	case 5: // queue 5 (bottom of the displayed queue)
		for (int i = 0; i < 4; i++) {
			minos[i].first = minos[i].first - global_offset_row + 15;
			minos[i].second = minos[i].second - global_offset_column + 19;
		}
		break;
	case 6: // hold piece
		if (shape != 0) {
			for (int i = 0; i < 4; i++) {
				minos[i].first = minos[i].first - global_offset_row + 3;
				minos[i].second = minos[i].second - global_offset_column + 2;
			}
		}
		break;
	}

	// update grid
	for (std::pair<int, int> mino : minos) {
		grid[mino.first][mino.second] = shape;
	}
	
}

// Checks if the Tetromino can move down
// Returns true if it can move down, false otherwise
bool Tetromino::canMove(int grid[][WINDOW_COLUMNS]) {
	bool canMove = true;
	
	for (int i = 0; i < 4; i++) {
		grid[minos[i].first][minos[i].second] = 0; // erase current minos from the grid
		minos[i].first++; // move Tetromino coordinates 1 down the board
	}

	// check if there is a collision
	for (std::pair<int, int> mino : minos) {
		// if any of the minos are past the bottom of the grid or collides with any piece (shape 1-7), the Tetromino cannot move
		if (mino.first >= BOARD_ROW_END || (grid[mino.first][mino.second] != 0 && grid[mino.first][mino.second] != 8)) canMove = false;
	}

	// move Tetromino coordinates back up 1
	for (int i = 0; i < 4; i++) {
		minos[i].first--;
		grid[minos[i].first][minos[i].second] = shape; // restore current minos to grid
	}

	return canMove;
}

// Moves the Tetromino down if possible
// Input:
//		canMove: true if the Tetromino can move down 1, false otherwise
void Tetromino::moveDown(int grid[][WINDOW_COLUMNS], bool canMove) {
	if (canMove) {
		// erase current minos
		for (int i = 0; i < 4; i++) {
			grid[minos[i].first][minos[i].second] = 0;
		}

		// update current minos after moving it down
		for (int i = 0; i < 4; i++) {
			minos[i].first++;
			grid[minos[i].first][minos[i].second] = shape;
		}
	}
}

// Moves the Tetromino left if possible
// Returns true if it can move left, false otherwise
bool Tetromino::moveLeft(int grid[][WINDOW_COLUMNS]) {
	bool canMove = true;
	
	for (int i = 0; i < 4; i++) {
		grid[minos[i].first][minos[i].second] = 0; // erase current minos from grid
		minos[i].second--; // move Tetromino coordinates left 1
	}

	// collision detection
	for (std::pair<int, int> mino : minos) {
		// if any of the minos are past the left side of the board or collides with any piece (shape 1-7), the Tetromino cannot move
		if (mino.second < BOARD_COLUMN_START || (grid[mino.first][mino.second] != 0 && grid[mino.first][mino.second] != 8)) canMove = false;
	}

	// update grid if piece can move, otherwise reset changes
	if (canMove) {
		for (std::pair<int, int> mino : minos) {
			grid[mino.first][mino.second] = shape;
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			minos[i].second++;
			grid[minos[i].first][minos[i].second] = shape;
		}
	}

	return canMove;
}

// Moves the Tetromino right if possible
// Returns true if it can move right, false otherwise
bool Tetromino::moveRight(int grid[][WINDOW_COLUMNS]) {
	bool canMove = true;

	for (int i = 0; i < 4; i++) {
		grid[minos[i].first][minos[i].second] = 0; // erase current minos from grid
		minos[i].second++; // move Tetromino coordinates right 1
	}

	// collision detection
	for (std::pair<int, int> mino : minos) {
		// if any of the minos are past the right side of the board or collides with any piece (shape 1-7), the Tetromino cannot move
		if (mino.second >= BOARD_COLUMN_END || (grid[mino.first][mino.second] != 0 && grid[mino.first][mino.second] != 8)) canMove = false;
	}

	// update grid if piece can move, otherwise reset changes
	if (canMove) {
		for (std::pair<int, int> mino : minos) {
			grid[mino.first][mino.second] = shape;
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			minos[i].second--;
			grid[minos[i].first][minos[i].second] = shape;
		}
	}

	return canMove;
}

// Creates the ghost piece for the Tetromino and moves it as far down as possible, updating the grid with its position
// Returns the coordinates of the ghost piece
std::pair<int, int>* Tetromino::updateGhost(int grid[][WINDOW_COLUMNS]) {
	// duplicate tetromino
	Tetromino ghost = { this->shape, this->rotation };
	for (int i = 0; i < 4; i++) {
		ghost.minos[i].first = minos[i].first;
		ghost.minos[i].second = minos[i].second;
	}

	bool canMove = true;
	while (canMove) {
		// move ghost piece down 1
		for (int i = 0; i < 4; i++) {
			ghost.minos[i].first++;
		}

		// check for collisions
		for (std::pair<int, int> mino : ghost.minos) {
			if (mino.first >= BOARD_ROW_END) canMove = false; // boundary check
			else if ((grid[mino.first][mino.second] != 0) && (grid[mino.first][mino.second] != 8)) { // collision detected
				bool isSelf = false; // check if the collision is with its own Tetromino
				for (int i = 0; i < 4; i++) {
					if (minos[i].first == mino.first && minos[i].second == mino.second) isSelf = true;
				}
				if (!isSelf) canMove = false; // the collision is with another piece
			}
		}
	}

	// update grid with ghost piece
	for (int i = 0; i < 4; i++) {
		ghost.minos[i].first--;
		// if the ghost mino overlaps with one of the original minos, the original mino takes precedence in the grid
		if (grid[ghost.minos[i].first][ghost.minos[i].second] != shape) grid[ghost.minos[i].first][ghost.minos[i].second] = 8;
	}

	return ghost.minos;
}

// Instantly moves the Tetromino down as far as possible by setting its coordinates to its ghost piece's coordinates
void Tetromino::hardDrop(int grid[][WINDOW_COLUMNS]) {
	std::pair<int, int>* ghostMinos = updateGhost(grid); // gets the ghost minos
	
	// erase current minos
	for (std::pair<int, int> mino : minos) {
		grid[mino.first][mino.second] = 0;
	}

	// sets current minos to ghost minos and update grid
	for (int i = 0; i < 4; i++) {
		minos[i].first = ghostMinos[i].first;
		minos[i].second = ghostMinos[i].second;
		grid[minos[i].first][minos[i].second] = shape;
	}
}

// Swaps the hold piece with the current piece (or sets the hold piece to the current piece if it's the first time hold is used in the current game)
// Input:
//		holdPiece: current hold piece passed by reference
// Returns true if there is a swap, false otherwise (returns false once per game)
bool Tetromino::hold(int grid[][WINDOW_COLUMNS], Tetromino &holdPiece) {
	bool swap;
	Tetromino newPiece = { shape, 0 }; // creates a new Tetromino with same shape as the current

	// erase current minos
	for (std::pair<int, int> mino : minos) {
		grid[mino.first][mino.second] = 0;
	}

	if (holdPiece.shape == 0) { // if there is no hold piece (once per game)
		holdPiece = newPiece; // set the hold piece to the new Tetromino
		holdPiece.setPiecePos(grid, 6);
		swap = false;
	}
	else { // swaps the current with the hold piece
		shape = holdPiece.shape;
		rotation = holdPiece.rotation;
		for (int i = 0; i < 4; i++) {
			minos[i].first = holdPiece.minos[i].first;
			minos[i].second = holdPiece.minos[i].second;
		}
		holdPiece = newPiece;

		setPiecePos(grid, 0); // update grid with current piece at spawn
		holdPiece.setPiecePos(grid, 6); // update grid with hold piece
		swap = true;
	}

	return swap;
}

// Rotates the Tetromino 90 degrees clockwise
// Returns true on successful rotation, false otherwise
bool Tetromino::rotateRight(int grid[][WINDOW_COLUMNS]) {
	if (shape == 2) return true; // don't rotate if shape is O piece
	std::vector<std::vector<int>> clockwiseMatrix{ {0, 1}, {-1, 0} }; // clockwise rotation matrix
	std::pair<int, int> rotatedMinos[4];
	int global_offset_row = minos[0].first;
	int global_offset_column = minos[0].second;

	// convert minos to relative coordinates
	for (int i = 0; i < 4; i++) {
		minos[i].first -= global_offset_row;
		minos[i].second -= global_offset_column;
	}

	// set rotatedMinos to new rotation coordinates
	for (int i = 0; i < 4; i++) {
		rotatedMinos[i].first = clockwiseMatrix[0][0] * minos[i].first + clockwiseMatrix[0][1] * minos[i].second;
		rotatedMinos[i].second = clockwiseMatrix[1][0] * minos[i].first + clockwiseMatrix[1][1] * minos[i].second;
	}

	int og_rotation = rotation; // original rotation
	rotation = mod(++rotation, 4); // new rotation

	// offset I piece
	if (shape == 1) {
		switch (rotation) {
		case 0:
			for (int i = 0; i < 4; i++) {
				rotatedMinos[i].first--;
			}
			break;
		case 1:
			for (int i = 0; i < 4; i++) {
				rotatedMinos[i].second++;
			}
			break;
		case 2:
			for (int i = 0; i < 4; i++) {
				rotatedMinos[i].first++;
			}
			break;
		case 3:
			for (int i = 0; i < 4; i++) {
				rotatedMinos[i].second--;
			}
			break;
		}
	}

	// convert both minos and rotatedMinos to global coordinates
	for (int i = 0; i < 4; i++) {
		rotatedMinos[i].first += global_offset_row;
		rotatedMinos[i].second += global_offset_column;
		minos[i].first += global_offset_row;
		minos[i].second += global_offset_column;
	}

	// erase current minos from grid
	for (std::pair<int, int> mino : minos) {
		grid[mino.first][mino.second] = 0;
	}

	// check wall kick data
	bool canRotate;
	int offset_row; // store each test's row offset
	int offset_column; // store each test's column offset
	for (int i = 0; i < 5; i++) { // loop through the 5 tests
		canRotate = true;
		if (shape != 1) { // JLSTZ pieces
			offset_row = JLSTZ_CLOCKWISE_WALLKICK_DATA[og_rotation][i][0];
			offset_column = JLSTZ_CLOCKWISE_WALLKICK_DATA[og_rotation][i][1];
		}
		else { // I piece
			offset_row = I_CLOCKWISE_WALLKICK_DATA[og_rotation][i][0];
			offset_column = I_CLOCKWISE_WALLKICK_DATA[og_rotation][i][1];
		}

		for (std::pair<int, int> mino : rotatedMinos) { // offset each mino and check if there is any collision
			if ((mino.second + offset_column) < BOARD_COLUMN_START || (mino.second + offset_column) >= BOARD_COLUMN_END || (mino.first + offset_row) >= BOARD_ROW_END
				|| (grid[mino.first + offset_row][mino.second + offset_column] != 0 && grid[mino.first + offset_row][mino.second + offset_column] != 8)) canRotate = false;
		}

		if (canRotate) {
			// sets rotatedMinos to test offset coordinates
			for (int i = 0; i < 4; i++) {
				rotatedMinos[i].first += offset_row;
				rotatedMinos[i].second += offset_column;
			}
			break; // don't check the remaining tests
		}
	}

	if (canRotate) { // set minos to rotatedMinos and update grid
		for (int i = 0; i < 4; i++) {
			minos[i].first = rotatedMinos[i].first;
			minos[i].second = rotatedMinos[i].second;
			grid[rotatedMinos[i].first][rotatedMinos[i].second] = shape;
		}
	}
	else { // if the Tetromino can't rotate, restore original minos to grid
		for (std::pair<int, int> mino : minos) {
			grid[mino.first][mino.second] = shape;
		}
		rotation = mod(--rotation, 4); // rotate back
	}

	return canRotate;
}

// Rotates the Tetromino 90 degrees counterclockwise
// Returns true on successful rotation, false otherwise
bool Tetromino::rotateLeft(int grid[][WINDOW_COLUMNS]) {
	if (shape == 2) return true; // don't rotate if shape is O piece
	std::vector<std::vector<int>> counterClockwiseMatrix{ {0, -1}, {1, 0} }; // counterclockwise rotation matrix
	std::pair<int, int> rotatedMinos[4];
	int global_offset_row = minos[0].first;
	int global_offset_column = minos[0].second;

	// convert minos to relative coordinates
	for (int i = 0; i < 4; i++) {
		minos[i].first -= global_offset_row;
		minos[i].second -= global_offset_column;
	}

	// set rotatedMinos to new rotation coordinates
	for (int i = 0; i < 4; i++) {
		rotatedMinos[i].first = counterClockwiseMatrix[0][0] * minos[i].first + counterClockwiseMatrix[0][1] * minos[i].second;
		rotatedMinos[i].second = counterClockwiseMatrix[1][0] * minos[i].first + counterClockwiseMatrix[1][1] * minos[i].second;
	}

	int og_rotation = rotation; // original rotation
	rotation = mod(--rotation, 4); // new rotation

	// offset I piece
	if (shape == 1) {
		switch (rotation) {
		case 0:
			for (int i = 0; i < 4; i++) {
				rotatedMinos[i].second--;
			}
			break;
		case 1:
			for (int i = 0; i < 4; i++) {
				rotatedMinos[i].first--;
			}
			break;
		case 2:
			for (int i = 0; i < 4; i++) {
				rotatedMinos[i].second++;
			}
			break;
		case 3:
			for (int i = 0; i < 4; i++) {
				rotatedMinos[i].first++;
			}
			break;
		}
	}

	// convert both minos and rotatedMinos to global coordinates
	for (int i = 0; i < 4; i++) {
		rotatedMinos[i].first += global_offset_row;
		rotatedMinos[i].second += global_offset_column;
		minos[i].first += global_offset_row;
		minos[i].second += global_offset_column;
	}

	// erase current minos from grid
	for (std::pair<int, int> mino : minos) {
		grid[mino.first][mino.second] = 0;
	}

	// check wall kick data
	bool canRotate;
	int offset_row; // store each test's row offset
	int offset_column; // store each test's column offset
	for (int i = 0; i < 5; i++) { // loop through the 5 tests
		canRotate = true;
		if (shape != 1) { // JLSTZ pieces
			offset_row = JLSTZ_COUNTERCLOCKWISE_WALLKICK_DATA[rotation][i][0];
			offset_column = JLSTZ_COUNTERCLOCKWISE_WALLKICK_DATA[rotation][i][1];
		}
		else { // I piece
			offset_row = I_COUNTERCLOCKWISE_WALLKICK_DATA[rotation][i][0];
			offset_column = I_COUNTERCLOCKWISE_WALLKICK_DATA[rotation][i][1];
		}

		for (std::pair<int, int> mino : rotatedMinos) { // offset each mino and check if there is any collision
			if ((mino.second + offset_column) < BOARD_COLUMN_START || (mino.second + offset_column) >= BOARD_COLUMN_END || (mino.first + offset_row) >= BOARD_ROW_END
				|| (grid[mino.first + offset_row][mino.second + offset_column] != 0 && grid[mino.first + offset_row][mino.second + offset_column] != 8)) canRotate = false;
		}

		if (canRotate) { // sets rotatedMinos to test offset coordinates
			for (int j = 0; j < 4; j++) {
				rotatedMinos[j].first += offset_row;
				rotatedMinos[j].second += offset_column;
			}
			break; // don't check the remaining tests
		}
	}

	if (canRotate) { // set minos to rotatedMinos and update grid
		for (int i = 0; i < 4; i++) {
			minos[i].first = rotatedMinos[i].first;
			minos[i].second = rotatedMinos[i].second;
			grid[rotatedMinos[i].first][rotatedMinos[i].second] = shape;
		}
	}
	else { // if the Tetromino can't rotate, restore original minos to grid
		for (std::pair<int, int> mino : minos) {
			grid[mino.first][mino.second] = shape;
		}
		rotation = mod(++rotation, 4); // rotate back
	}

	return canRotate;
}

// "True" modulo method that works with negative numbers as well
int Tetromino::mod(int x, int m) {
	return (x % m + m) % m;
}
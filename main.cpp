#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <deque>
#include <SFML/Graphics.hpp>
#include "grid_data.h"
#include "Tetromino.h"


// 7 piece bag randomizer
void generateBag(std::deque<Tetromino>& pieceQueue) {
    std::vector<int> pieces;
    for (int i = 1; i <= 7; i++) pieces.push_back(i);
    std::random_shuffle(pieces.begin(), pieces.end());
    
    for (int i : pieces) {
        Tetromino piece = { i, 0 };
        pieceQueue.push_back(piece);
    }
}

// update grid with current queue pieces
void updateQueue(int grid[][WINDOW_COLUMNS], std::deque<Tetromino>& pieceQueue) {
    for (int i = 0; i < 5; i++) {
        pieceQueue[i].setPiecePos(grid, i + 1);
    }
}

// clear the queue section of the grid
void resetQueue(int grid[][WINDOW_COLUMNS]) {
    for (int i = 2; i <= 15; i++) {
        for (int j = 18; j <= 21; j++) {
            grid[i][j] = 0;
        }
    }
}

// clear the hold section of the grid
void resetHold(int grid[][WINDOW_COLUMNS]) {
    for (int i = 2; i <= 3; i++) {
        for (int j = 1; j <= 4; j++) {
            grid[i][j] = 0;
        }
    }
}

// clear any ghost minos on the board
void resetGhost(int grid[][WINDOW_COLUMNS]) {
    for (int i = 0; i < BOARD_ROW_END; i++) {
        for (int j = BOARD_COLUMN_START; j < BOARD_COLUMN_END; j++) {
            if (grid[i][j] == 8) grid[i][j] = 0;
        }
    }
}

// check the board for any full rows of minos
void checkLineClear(int grid[][WINDOW_COLUMNS]) {
    for (int i = BOARD_ROW_START; i < BOARD_ROW_END; i++) { // loop through rows from top to bottom
        bool clear = true;
        for (int j = BOARD_COLUMN_START; j < BOARD_COLUMN_END; j++) { // loop through the columns
            if (grid[i][j] == 0) { // if there is any gap in the columns, it's not a line clear
                clear = false;
            }
        }

        if (clear) {
            for (int m = i; m >= BOARD_ROW_START; m--) { // loop from current row (line clear) to top of the board
                for (int n = BOARD_COLUMN_START; n < BOARD_COLUMN_END; n++) {
                    grid[m][n] = grid[m - 1][n]; // replace each mino in the current row with the mino above it
                }
            }
        }
    }
}

// initialize a new game
void initializeGame(int grid[][WINDOW_COLUMNS], std::deque<Tetromino>& pieceQueue, Tetromino& currentPiece, Tetromino& holdPiece,
                    std::chrono::time_point<std::chrono::steady_clock>& prev_time, std::chrono::time_point<std::chrono::steady_clock>& prev_lock_time) {
    memset(grid, 0, sizeof(grid[0][0]) * WINDOW_ROWS * WINDOW_COLUMNS); // zero out grid
    // empty pieceQueue by swapping it with an empty queue
    std::deque<Tetromino> empty;
    std::swap(pieceQueue, empty);
    // reset current and hold pieces
    currentPiece = Tetromino(0, 0);
    holdPiece = currentPiece;

    generateBag(pieceQueue); // generate 7 piece bag
    currentPiece = pieceQueue.front(); // set currentPiece to Tetromino at front of queue
    pieceQueue.pop_front();

    // update grid with the positions of currentPiece and each piece in the queue
    currentPiece.setPiecePos(grid, 0);
    updateQueue(grid, pieceQueue);

    // start clocks for gravity and Tetromino lock time
    prev_time = std::chrono::steady_clock::now();
    prev_lock_time = std::chrono::steady_clock::now();
}

// check if the game is over (if a piece is placed above the top of the board)
bool checkGameOver(int grid[][WINDOW_COLUMNS], bool piecePlaced) {
    bool gameOver = false;
    for (int j = BOARD_COLUMN_START; j < BOARD_COLUMN_END; j++) {
        if (piecePlaced && grid[1][j] != 0) gameOver = true; // if a piece was placed, check if any mino is above the top of the board
    }
    return gameOver;
}

int main() {
    int grid[WINDOW_ROWS][WINDOW_COLUMNS] = {};
    bool hasFocus;
    bool canMoveDown;
    bool piecePlaced = false, holdUsed = false, gameOver = false;
    int gravity_time, lock_time, max_lock_time = 0;
    std::chrono::time_point<std::chrono::steady_clock> prev_time, prev_lock_time, prev_max_lock_time;
    std::deque<Tetromino> pieceQueue;
    Tetromino currentPiece = { 0, 0 };
    Tetromino holdPiece = { 0, 0 };
    sf::Color colors[9] = { sf::Color(50, 50, 50),     // gray background (no mino)
                           sf::Color(0, 255, 255),     // cyan I
                           sf::Color(255, 255, 0),     // yellow O
                           sf::Color(128, 0, 128),     // purple T
                           sf::Color(0, 255, 0),       // green S
                           sf::Color(255, 0, 0),       // red Z
                           sf::Color(255, 127, 0),     // orange L
                           sf::Color(0, 0, 255),       // blue J
                           sf::Color(127, 127, 127) }; // gray ghost

    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * (WINDOW_COLUMNS), CELL_SIZE * (WINDOW_ROWS)), "Tetris", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    srand(time(NULL)); // randomize seed
    initializeGame(grid, pieceQueue, currentPiece, holdPiece, prev_time, prev_lock_time); // start game

    while (window.isOpen()) {
        sf::Event event;
            
        canMoveDown = currentPiece.canMove(grid);
        if (!canMoveDown) { // can't move down = piece is currently resting on some mino(s)
            // time spent resting on a mino and since last action (moving/rotating)
            lock_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - prev_lock_time).count();

            // total time spent resting on a mino
            max_lock_time += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - prev_max_lock_time).count();
            prev_max_lock_time = std::chrono::steady_clock::now();

            // if no action is taken for .5 seconds or total time spent resting on a mino is 7+ seconds, place the piece
            if (lock_time >= 500000 || max_lock_time >= 7000000) piecePlaced = true;
        }
        else { // piece is not currently resting on any mino
            prev_lock_time = std::chrono::steady_clock::now();
            prev_max_lock_time = std::chrono::steady_clock::now();
        }

        // try to move the current piece down every .5 seconds
        gravity_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - prev_time).count(); // time in microseconds
        if (gravity_time >= 500000) {
            currentPiece.moveDown(grid, canMoveDown);
            prev_time += std::chrono::microseconds(gravity_time);
        }

        gameOver = checkGameOver(grid, piecePlaced); // check if game over
        while (gameOver) { // if game is over, wait indefinitely until R is pressed to start a new game
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) { // restart the game
                    initializeGame(grid, pieceQueue, currentPiece, holdPiece, prev_time, prev_lock_time);
                    piecePlaced = false;
                    holdUsed = false;
                    gameOver = false;
                    max_lock_time = 0;
                }
            }
        }

        // if the current piece is placed, set currentPiece to next piece in queue
        if (piecePlaced) {
            checkLineClear(grid);
            currentPiece = pieceQueue.front();
            pieceQueue.pop_front();
            if (pieceQueue.size() < 5) generateBag(pieceQueue); // if there are less than 5 pieces in the queue, generate the next 7 piece bag

            // update the grid
            resetQueue(grid);
            currentPiece.setPiecePos(grid, 0);
            updateQueue(grid, pieceQueue);

            piecePlaced = false;
            holdUsed = false;
            max_lock_time = 0;
        }

        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::LostFocus:
                hasFocus = false;
                break;
            case sf::Event::GainedFocus:
                hasFocus = true;
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Down: // move down if possible (soft drop)
                    canMoveDown = currentPiece.canMove(grid);
                    currentPiece.moveDown(grid, canMoveDown);
                    break;
                case sf::Keyboard::Left: // move left if possible
                    canMoveDown = currentPiece.canMove(grid);
                    if (!canMoveDown) prev_time += std::chrono::microseconds(gravity_time); // moving left resets gravity timer if it's immediately above any block
                    if (currentPiece.moveLeft(grid)) prev_lock_time = std::chrono::steady_clock::now(); // moving left resets lock timer
                    else if (!canMoveDown) prev_time -= std::chrono::microseconds(gravity_time); // if can't move left or down, restore gravity timer (no action)
                    break;
                case sf::Keyboard::Right: // move right if possible
                    canMoveDown = currentPiece.canMove(grid);
                    if (!canMoveDown) prev_time += std::chrono::microseconds(gravity_time); // moving right resets gravity timer if it's immediately above any block
                    if (currentPiece.moveRight(grid)) prev_lock_time = std::chrono::steady_clock::now(); // moving right resets lock timer
                    else if (!canMoveDown) prev_time -= std::chrono::microseconds(gravity_time); // if can't move right or down, restore gravity timer (no action)
                    break;
                case sf::Keyboard::Space: // hard drop (lock piece into place)
                    currentPiece.hardDrop(grid);
                    piecePlaced = true;
                    break;
                case sf::Keyboard::LShift: // swap current and hold pieces if possible
                    if (!holdUsed) { // can only hold once per piece
                        resetHold(grid);
                        if (!currentPiece.hold(grid, holdPiece)) { // if there is no hold piece, queue also needs to be updated
                            resetQueue(grid);
                            currentPiece = pieceQueue.front();
                            pieceQueue.pop_front();
                            if (pieceQueue.size() < 5) generateBag(pieceQueue);

                            currentPiece.setPiecePos(grid, 0);
                            updateQueue(grid, pieceQueue);
                        }
                        holdUsed = true;
                    }
                    break;
                case sf::Keyboard::Up: // rotate clockwise
                    canMoveDown = currentPiece.canMove(grid);
                    if (!canMoveDown) prev_time += std::chrono::microseconds(gravity_time); // rotating won't reset gravity timer if it can move down
                    if (currentPiece.rotateRight(grid)) prev_lock_time = std::chrono::steady_clock::now(); // successful rotation resets lock timer
                    else if (!canMoveDown) prev_time -= std::chrono::microseconds(gravity_time); // if can't rotate and can't move down, restore gravity timer (no action)
                    break;
                case sf::Keyboard::X: // rotate counterclockwise
                    canMoveDown = currentPiece.canMove(grid);
                    if (!canMoveDown) prev_time += std::chrono::microseconds(gravity_time); // rotating won't reset gravity timer if it can move down
                    if (currentPiece.rotateLeft(grid)) prev_lock_time = std::chrono::steady_clock::now(); // successful rotation resets lock timer
                    else if (!canMoveDown) prev_time -= std::chrono::microseconds(gravity_time); // if can't rotate and can't move down, restore gravity timer (no action)
                    break;
                case sf::Keyboard::R: // reset the game
                    initializeGame(grid, pieceQueue, currentPiece, holdPiece, prev_time, prev_lock_time);
                    piecePlaced = false;
                    holdUsed = false;
                    gameOver = false;
                    max_lock_time = 0;
                    break;
                }
            }
        }

        // continously update ghost piece
        resetGhost(grid);
        currentPiece.updateGhost(grid);

        window.clear();

        sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        cell.setOutlineColor(sf::Color::Black);
        cell.setOutlineThickness(1.f);
        // draw the entire grid
        for (int i = 0; i < WINDOW_ROWS; i++) {
            for (int j = 0; j < WINDOW_COLUMNS; j++) {
                cell.setFillColor(sf::Color(colors[grid[i][j]])); // set cell color to corresponding shape
                cell.setPosition((j)*CELL_SIZE, (i)*CELL_SIZE);

                // only draw the board cells and tetrominos in queue/hold/spawn
                if ((i >= BOARD_ROW_START && j >= BOARD_COLUMN_START && j < BOARD_COLUMN_END) || (grid[i][j] != 0)) window.draw(cell);
            }
        }

        window.display();
    }

    return 0;
}
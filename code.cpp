#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include "raylib.h"

using namespace std;

enum CellStates { DEATH, LIVE };

class GameOfLife {
private:
    int gen;
    int totalCells;
    int liveCells;
    int X_DIM;
    int Y_DIM;
    int cellSize;
    float updateTimer;
    float updateInterval;
    vector<vector<CellStates>> grid;

public:
    GameOfLife(int x = 60, int y = 100, int cSize = 10);
    void initializeGrid();
    int nextLiveCells(int i, int j);
    void nextGen();
    int countLiveCells();
    void drawGrid();
    void start();
};

GameOfLife :: GameOfLife(int x, int y, int cSize) {
    X_DIM = x;
    Y_DIM = y;
    cellSize = cSize;
    totalCells = X_DIM * Y_DIM;
    gen = 0;
    liveCells = 0;
    updateTimer = 0.0f;
    updateInterval = 0.05f;
    initializeGrid();
}

void GameOfLife :: initializeGrid() {
    srand(time(nullptr));
    grid = vector<vector<CellStates>>(X_DIM, vector<CellStates>(Y_DIM));
    for (int i = 0; i < X_DIM; ++i) {
        for (int j = 0; j < Y_DIM; ++j) {
            grid[i][j] = static_cast<CellStates>(rand() % 2);
        }
    }
}

int GameOfLife :: nextLiveCells(int i, int j) {
    int lives = 0;
    for (int k = -1; k <= 1; ++k) {
        for (int h = -1; h <= 1; ++h) {
            if (k == 0 && h == 0) continue;
            int x = (k + i + X_DIM) % X_DIM;
            int y = (h + j + Y_DIM) % Y_DIM;
            lives += static_cast<int>(grid[x][y]);
        }
    }
    return lives;
}

void GameOfLife :: nextGen() {
    vector<vector<CellStates>> tmpGrid(X_DIM, vector<CellStates>(Y_DIM));
    for (int i = 0; i < X_DIM; i++) {
        for (int j = 0; j < Y_DIM; j++) {
            int neighbors = nextLiveCells(i, j);
            if (grid[i][j] == LIVE && (neighbors == 2 || neighbors == 3)) {
                tmpGrid[i][j] = LIVE;
            } else if (grid[i][j] == DEATH && neighbors == 3) {
                tmpGrid[i][j] = LIVE;
            } else {
                tmpGrid[i][j] = DEATH;
            }
        }
    }
    grid = tmpGrid;
    gen++;
    liveCells = countLiveCells();
}

int GameOfLife :: countLiveCells() {
    int count = 0;
    for (int i = 0; i < X_DIM; i++) {
        for (int j = 0; j < Y_DIM; j++) {
            if (grid[i][j] == LIVE) count++;
        }
    }
    return count;
}

void GameOfLife :: drawGrid() {
    for (int i = 0; i < X_DIM; i++) {
        for (int j = 0; j < Y_DIM; j++) {
            if (grid[i][j] == LIVE) {
                DrawRectangle(j * cellSize, i * cellSize, cellSize - 1, cellSize - 1, GREEN);
            }
        }
    }
}

void GameOfLife::start() {
    int screenWidth = Y_DIM * cellSize;
    int screenHeight = X_DIM * cellSize;
    InitWindow(screenWidth, screenHeight, "Game of Life");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        updateTimer += deltaTime;
        if (updateTimer >= updateInterval) {
            nextGen();
            updateTimer = 0.0f;
        }
        BeginDrawing();
            ClearBackground(BLACK);
            drawGrid();
        EndDrawing();
    }
}

int main() {
    GameOfLife game(60, 100, 10);
    game.start();
    return 0;
}

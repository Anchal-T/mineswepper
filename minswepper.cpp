#include <iostream>
#include <raylib.h>

#define ROWS 10
#define COLS 10

const int screenHeight = 400;
const int screenWidth = 400;

const int cellHeight = screenHeight / ROWS;
const int cellWidth = screenWidth / COLS;

typedef struct Cell
{
	int i;
	int j;
} Cell;

Cell grid[ROWS][COLS];

void DrawCells(Cell);

int main(int argc, char const *argv[])
{
	InitWindow(screenHeight, screenWidth, "A window");

	for(int i=0; i<ROWS; i++){
		for(int j=0; j<COLS; j++){
			grid[i][j] = (Cell){
				.i = i,
				.j = j
			};
		}
	}

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);
		for (int i = 0; i < ROWS; ++i)
		{
			for (int j = 0; j < COLS; ++j)
			{
				DrawCells(grid[i][j]);
			}
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

void DrawCells(Cell cell)
{
	DrawRectangleLines(cell.i * cellWidth, cell.j * cellHeight, cellWidth, cellHeight, LIGHTGRAY);
}
#include <iostream>
#include <stdlib.h>
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
	bool containsMine;
	bool revealed;
	int MinesNearby;
} Cell;

Cell grid[ROWS][COLS];

void DrawCells(Cell);
bool IsValid(int, int);
void IsRevealed(int, int);
int countMines(int, int);

int main(int argc, char const *argv[])
{
	InitWindow(screenHeight, screenWidth, "A window");

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			grid[i][j] = (Cell){
				.i = i,
				.j = j,
				.containsMine = false,
				.revealed = false,
				.MinesNearby = -1};
		}
	}

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			grid[i][j].MinesNearby = countMines(i, j);
		}
	}

	int mineToPlace = (int)(ROWS * COLS * 0.1f);
	while (mineToPlace > 0)
	{
		int i = rand() % COLS;
		int j = rand() % ROWS;

		if (!grid[i][j].containsMine)
		{
			grid[i][j].containsMine = true;
			mineToPlace--;
		}
	}

	while (!WindowShouldClose())
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			Vector2 mPos = GetMousePosition();
			int IndexI = mPos.x / cellWidth;
			int IndexJ = mPos.y / cellWidth;

			if (IsValid(IndexI, IndexJ))
			{
				IsRevealed(IndexI, IndexJ);
			}
		}
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
	if (cell.revealed)
	{
		if (cell.containsMine)
		{
			DrawRectangle(cell.i * cellWidth, cell.j * cellHeight, cellWidth, cellHeight, RED);
		}
		else
		{
			DrawRectangle(cell.i * cellWidth, cell.j * cellHeight, cellWidth, cellHeight, GRAY);
		}
	}

	DrawRectangleLines(cell.i * cellWidth, cell.j * cellHeight, cellWidth, cellHeight, LIGHTGRAY);
}

bool IsValid(int I, int J)
{
	if (I >= 0 && I < COLS && J >= 0 && J < ROWS)
	{
		return true;
	}
	return false;
}

void IsRevealed(int I, int J)
{
	grid[I][J].revealed = true;
}

int countMines(int i, int j)
{
	int count = 0;

	for (int I = -1; I <= 1; I++)
	{
		for (int J = -1; J <= 1; J++)
		{
			if (I == 0 && J == 0)
			{
				continue;
			}
			else if (!IsValid(i + I, j + J))
			{
				continue;
			}
			else if (grid[i + I][j + J].containsMine)
			{
				count++;
			}
		}
	}
	return count;
}

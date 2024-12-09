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
	bool flag;
} Cell;

Cell grid[ROWS][COLS];

void DrawCells(Cell);
bool IsValid(int, int);
void IsRevealed(int, int);
int countMines(int, int);
void IsCellFlagged(int, int);

Texture2D flagTexture;
Texture2D bombTexture;

int main(int argc, char const *argv[])
{
	InitWindow(screenHeight, screenWidth, "Minswepper");

	Image flagImage = LoadImage("resources/minswepper_flag.png");
	flagTexture = LoadTextureFromImage(flagImage);
	UnloadImage(flagImage);

	Image bombImage = LoadImage("resources/minswepper_bomb.png");
	bombTexture = LoadTextureFromImage(bombImage);
	UnloadImage(bombImage);

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			grid[i][j] = (Cell){
				.i = i,
				.j = j,
				.containsMine = false,
				.revealed = false,
				.MinesNearby = -1,
				.flag = false};
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

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			grid[i][j].MinesNearby = countMines(i, j);
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
		else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			Vector2 mPos = GetMousePosition();
			int IndexI = mPos.x / cellWidth;
			int IndexJ = mPos.y / cellWidth;

			if (IsValid(IndexI, IndexJ))
			{
				IsCellFlagged(IndexI, IndexJ);
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
			Rectangle src = {0, 0, (float)bombTexture.width, (float)bombTexture.height};
			Rectangle dest = {(float)cell.i * cellWidth, (float)cell.j * cellHeight, cellWidth, cellHeight};
			Vector2 origin = {0, 0};

			DrawTexturePro(bombTexture, src, dest, origin, 0, WHITE);
		}
		else
		{
			DrawRectangle(cell.i * cellWidth, cell.j * cellHeight, cellWidth, cellHeight, GRAY);
			if (cell.MinesNearby)
			{
				DrawText(TextFormat("%d", cell.MinesNearby), cell.i * cellWidth + 10, cell.j * cellHeight + 6, 25, RED);
			}
		}
	}
	else if (cell.flag)
	{
		Rectangle src = {0, 0, (float)flagTexture.width, (float)flagTexture.height};
		Rectangle dest = {(float)cell.i * cellWidth, (float)cell.j * cellHeight, cellWidth, cellHeight};
		Vector2 origin = {0, 0};

		DrawTexturePro(flagTexture, src, dest, origin, 0, RED);
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
	if (grid[I][J].flag || grid[I][J].revealed)
	{
		return;
	}
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

void IsCellFlagged(int i, int j)
{
	if (grid[i][j].revealed)
	{
		return;
	}

	grid[i][j].flag = !grid[i][j].flag;
}

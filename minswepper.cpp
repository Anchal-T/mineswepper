#include <iostream>
#include <stdlib.h>
#include <raylib.h>

#define ROWS 10
#define COLS 10

const int headerHeight = 60;  // Add header space
const int screenHeight = 400 + headerHeight;  // Increase total height
const int screenWidth = 400;

const int cellHeight = (screenHeight - headerHeight) / ROWS;  // Adjust cell height
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

typedef enum state
{	
	PLAYING,
	WIN,
	LOSE
}GameState;

GameState state;

Cell grid[ROWS][COLS];

void DrawCells(Cell);
bool IsValid(int, int);
void IsRevealed(int, int);
int countMines(int, int);
void IsCellFlagged(int, int);
void ClearGrid(int, int);
void game_init();
void RevealAllMines();
void CheckWinCondition();
void PlaceMines(int, int);

Texture2D flagTexture;
Texture2D bombTexture;

float gameStartTime;
float gameEndTime;
bool firstClick = true;

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    InitWindow(screenWidth, screenHeight, "Minswepper");  // Use new height

    Image flagImage = LoadImage("resources/minswepper_flag.png");
    flagTexture = LoadTextureFromImage(flagImage);
    UnloadImage(flagImage);

    Image bombImage = LoadImage("resources/minswepper_bomb.png");
    bombTexture = LoadTextureFromImage(bombImage);
    UnloadImage(bombImage);

    game_init();

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mPos = GetMousePosition();
            int IndexI = mPos.x / cellWidth;
            int IndexJ = (mPos.y - headerHeight) / cellHeight;  // Adjust for header

            if (IsValid(IndexI, IndexJ) && mPos.y >= headerHeight)  // Check if click is below header
            {
                IsRevealed(IndexI, IndexJ);
            }
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            Vector2 mPos = GetMousePosition();
            int IndexI = mPos.x / cellWidth;
            int IndexJ = (mPos.y - headerHeight) / cellHeight;  // Adjust for header

            if (IsValid(IndexI, IndexJ) && mPos.y >= headerHeight)  // Check if click is below header
            {
                IsCellFlagged(IndexI, IndexJ);
            }
        }
        else if(IsKeyPressed(KEY_R)){
            game_init();
        }
        
        BeginDrawing();
        ClearBackground(WHITE);
        
        // Draw header background
        DrawRectangle(0, 0, screenWidth, headerHeight, LIGHTGRAY);
        DrawLine(0, headerHeight, screenWidth, headerHeight, DARKGRAY);
        
        // Draw timer
        if(state == PLAYING) {
            float currentTime = GetTime() - gameStartTime;
            DrawText(TextFormat("Time: %.1f", currentTime), 10, 15, 20, BLACK);
        } else {
            float finalTime = gameEndTime - gameStartTime;
            DrawText(TextFormat("Time: %.1f", finalTime), 10, 15, 20, BLACK);
        }
        
        // Draw mine counter
        int totalMines = (int)(ROWS * COLS * 0.1f);
        int flaggedCells = 0;
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (grid[i][j].flag) flaggedCells++;
            }
        }
        int remainingMines = totalMines - flaggedCells;
        DrawText(TextFormat("Mines: %d", remainingMines), screenWidth - 120, 15, 20, BLACK);
        
        // Draw restart instruction
        DrawText("Press R to restart", screenWidth/2 - 80, 35, 16, DARKGRAY);
        
        // Draw grid with offset
        for (int i = 0; i < ROWS; ++i)
        {
            for (int j = 0; j < COLS; ++j)
            {
                DrawCells(grid[i][j]);
            }
        }
        
        if(state == LOSE){
            DrawRectangle(screenWidth/2 - 120, screenHeight/2 - 30, 240, 60, Color{0, 0, 0, 180});
            DrawText("Game Over", screenWidth/2 - 100, screenHeight/2 - 20, 40, RED);
        } else if(state == WIN) {
            DrawRectangle(screenWidth/2 - 100, screenHeight/2 - 30, 200, 60, Color{0, 0, 0, 180});
            DrawText("You Win!", screenWidth/2 - 80, screenHeight/2 - 20, 40, GREEN);
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void DrawCells(Cell cell)
{	
    // Offset all cell drawing by headerHeight
    int cellX = cell.i * cellWidth;
    int cellY = cell.j * cellHeight + headerHeight;
        
    if (cell.revealed)
    {
        if (cell.containsMine)
        {
            Rectangle src = {0, 0, (float)bombTexture.width, (float)bombTexture.height};
            Rectangle dest = {(float)cellX, (float)cellY, cellWidth, cellHeight};
            Vector2 origin = {0, 0};

            DrawTexturePro(bombTexture, src, dest, origin, 0, WHITE);
        }
        else
        {
            DrawRectangle(cellX, cellY, cellWidth, cellHeight, GRAY);
            if (cell.MinesNearby)
            {
                DrawText(TextFormat("%d", cell.MinesNearby), cellX + 10, cellY + 6, 25, RED);
            }
        }
    }
    else if (cell.flag)
    {
        Rectangle src = {0, 0, (float)flagTexture.width, (float)flagTexture.height};
        Rectangle dest = {(float)cellX, (float)cellY, cellWidth, cellHeight};
        Vector2 origin = {0, 0};

        DrawTexturePro(flagTexture, src, dest, origin, 0, RED);
    }

    DrawRectangleLines(cellX, cellY, cellWidth, cellHeight, LIGHTGRAY);
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
	if (grid[I][J].flag || grid[I][J].revealed || state != PLAYING)
	{
		return;
	}

	if(firstClick){
		firstClick = false;
		PlaceMines(I, J);

		for(int i=0; i<ROWS; i++){
			for(int j=0; j<COLS; j++){
				grid[i][j].MinesNearby = countMines(i, j);
			}
		}
	}

	grid[I][J].revealed = true;

	if(grid[I][J].containsMine){
		state = LOSE;
		gameEndTime = GetTime();
		RevealAllMines();
	}

	if(grid[I][J].MinesNearby == 0){
		ClearGrid(I, J);
	}
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

void ClearGrid(int i, int j){
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
			IsRevealed(i + I, j + J);
		}
	}
}

void RevealAllMines()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (grid[i][j].containsMine) {
				grid[i][j].revealed = true;
			}
		}
	}
}

void CheckWinCondition()
{
	int totalCells = ROWS * COLS;
	int revealedCells = 0;
	int mineCount = 0;
	
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (grid[i][j].containsMine) {
				mineCount++;
			}
			if (grid[i][j].revealed) {
				revealedCells++;
			}
		}
	}
	
	if (revealedCells + mineCount == totalCells) {
		state = WIN;
		gameEndTime = GetTime();
	}
}

void game_init(){
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
				.flag = false
			};
		}
	}
	state = PLAYING;
	gameStartTime = GetTime();
	gameEndTime = 0;
	firstClick = true;
	// int mineToPlace = (int)(ROWS * COLS * 0.1f);
	// while (mineToPlace > 0)
	// {
	// 	int i = rand() % COLS;
	// 	int j = rand() % ROWS;

	// 	if (!grid[i][j].containsMine)
	// 	{
	// 		grid[i][j].containsMine = true;
	// 		mineToPlace--;
	// 	}
	// }

	// for (int i = 0; i < ROWS; i++)
	// {
	// 	for (int j = 0; j < COLS; j++)
	// 	{
	// 		grid[i][j].MinesNearby = countMines(i, j);
	// 	}
	// }

}

void PlaceMines(int avoidI, int avoidJ){
	int mineToPlace = (int)(ROWS * COLS * 0.1f);
	while(mineToPlace > 0){
		int i = rand() % COLS;
		int j = rand() % ROWS;

		bool tooClose = false;
		for(int ti=-1; ti<=1; ti++){
			for(int tj=-1; tj<=1; tj++){
				if(ti == avoidI + ti && tj == avoidJ + tj){
					tooClose = true;
					break;
				}
			}
			if(tooClose) break;
		}

		if(!grid[i][j].containsMine && !(i == avoidI && j == avoidJ)){
			grid[i][j].containsMine = true;
			mineToPlace--;
		}
	}
}
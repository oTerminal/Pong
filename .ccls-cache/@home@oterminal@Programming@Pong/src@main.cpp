#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#define STORAGE_DATA_FILE    "storage.data"

typedef enum {
    STORAGE_POSITION_SCORE,
    STORAGE_POSITION_HISCORE
} StorageData;

static bool SaveStorageValue(unsigned int position, int value);
static int LoadStorageValue(unsigned int position);

struct Ball
{
	float x,y;
	float speedX, speedY;
	float radius;

	void Draw()
	{
		DrawCircle(x, y, radius, WHITE);
	}
};

struct Paddle
{
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect()
	{
		return Rectangle{x, y, width, height};
	}

	void Draw()
	{
		DrawRectangleRec(GetRect(), WHITE);
	}
};

int main(void)
{
	std::ofstream ("storage.data");
    const int screenWidth = 800;
    const int screenHeight = 600;

	Ball ball;
	ball.x = screenWidth / 2.0f;
	ball.y = screenHeight / 2.0f;
	ball.radius = 10;
	ball.speedX = 300;
	ball.speedY = 300;

	Paddle leftPaddle;
	leftPaddle.x = 30;
	leftPaddle.y = screenHeight / 2.0f - 60;
	leftPaddle.width = 20;
	leftPaddle.height = 120;
	leftPaddle.speed = 550;

	Paddle rightPaddle;
	rightPaddle.x = screenWidth - 20 - 30;
	rightPaddle.y = screenHeight / 2.0f - 60;
	rightPaddle.width = 20;
	rightPaddle.height = 120;
	rightPaddle.speed = 550;

	const char* winnerText = nullptr;
	int lScore = 0;
	int rScore = 0;

	Rectangle topBorder = {0, 0, screenWidth, 0 };
	Rectangle bottomBorder = {0, screenHeight, screenWidth, 1};

	

    InitWindow(screenWidth, screenHeight, "Pong");
	InitAudioDevice();

	Sound hitPaddle = LoadSound("../resources/ping_pong_8bit_plop.ogg");

    SetTargetFPS(300);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		if(ball.y < 0)
		{
			ball.y = 0;
			ball.speedY *= -1;
		}

		if(ball.y > GetScreenHeight())
		{
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}

		if(IsKeyDown(KEY_W))
		{
			if(leftPaddle.y != 0)
			{
				leftPaddle.y -= leftPaddle.speed * GetFrameTime();
			}
		}

		if(IsKeyDown(KEY_S))
		{
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}

		if(IsKeyDown(KEY_UP))
		{
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}

		if(IsKeyDown(KEY_DOWN))
		{
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}

		if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, rightPaddle.GetRect()))
		{
			if(ball.speedX > 0)
			{
				PlaySound(hitPaddle);
				ball.speedX *= -1.0f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * ball.speedX;
			}
		}

		if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, leftPaddle.GetRect()))
		{
			if(ball.speedX < 0)
			{
				PlaySound(hitPaddle);
				ball.speedX *= -1.0f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}

		if(CheckCollisionRecs(leftPaddle.GetRect(), topBorder))
		{
			leftPaddle.y = 0;
		}

		if(CheckCollisionRecs(rightPaddle.GetRect(), topBorder))
		{
			rightPaddle.y = 0;
		}

		if(CheckCollisionRecs(leftPaddle.GetRect(), bottomBorder))
		{
			leftPaddle.y = screenHeight - leftPaddle.height;
		}

		if(CheckCollisionRecs(rightPaddle.GetRect(), bottomBorder))
		{
			rightPaddle.y = screenHeight - rightPaddle.height;
		}

		if(ball.x > screenWidth)
		{
			winnerText = "Left Player Wins!";
			ball.x += ball.x;
			ball.y += ball.y;
			ball.x = GetScreenWidth() / 2.0f;
			ball.y = GetScreenHeight() / 2.0f - 100;
			ball.speedX = 0;
			ball.speedY = 0;

			lScore++;
			SaveStorageValue(STORAGE_POSITION_SCORE, lScore);
			SaveStorageValue(STORAGE_POSITION_HISCORE, rScore);
		}

		if(ball.x < 0)
		{
			winnerText = "Right Player Wins!";
			ball.x += ball.x;
			ball.y += ball.y;
			ball.x = GetScreenWidth() / 2.0f;
			ball.y = GetScreenHeight() / 2.0f - 100;
			ball.speedX = 0;
			ball.speedY = 0;

			rScore++;
			SaveStorageValue(STORAGE_POSITION_SCORE, lScore);
			SaveStorageValue(STORAGE_POSITION_HISCORE, rScore);
		}

		if(winnerText && IsKeyPressed(KEY_ENTER))
		{
			ball.x = GetScreenWidth() / 2.0f;
			ball.y = GetScreenHeight() / 2.0f;
			ball.speedX = 250;
			ball.speedY = 250;
			winnerText = nullptr;

			leftPaddle.x = 30;
			leftPaddle.y = screenHeight / 2.0f - 60;
			leftPaddle.width = 20;
			leftPaddle.height = 120;
			leftPaddle.speed = 550;

			rightPaddle.x = screenWidth - 20 - 30;
			rightPaddle.y = screenHeight / 2.0f - 60;
			rightPaddle.width = 20;
			rightPaddle.height = 120;
			rightPaddle.speed = 550;

		}

        lScore = LoadStorageValue(STORAGE_POSITION_SCORE);
        rScore = LoadStorageValue(STORAGE_POSITION_HISCORE);
	
		BeginDrawing();

            ClearBackground(BLACK);
			DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, GREEN);
			DrawText(TextFormat("%i", lScore), 200, 60, 60, RED);
			DrawText(TextFormat("%i", rScore), 600, 60, 60, RED);

			ball.Draw();
			leftPaddle.Draw();
			rightPaddle.Draw();

			if(winnerText)
			{

				int winnerTextWidth = MeasureText(winnerText, 60);
				DrawText(winnerText, GetScreenWidth() / 2 - winnerTextWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
				DrawText(TextFormat("%i", lScore), 200, 60, 60, RED);
				DrawText(TextFormat("%i", rScore), 600, 60, 60, RED);

			}

        EndDrawing();
    }

	UnloadSound(hitPaddle);
	CloseAudioDevice(); 
	remove("storage.data");
    CloseWindow();
	return 0;
}

// Save integer value to storage file (to defined position)
// NOTE: Storage positions is directly related to file memory layout (4 bytes each integer)
bool SaveStorageValue(unsigned int position, int value)
{
    bool success = false;
    unsigned int dataSize = 0;
    unsigned int newDataSize = 0;
    unsigned char *fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);
    unsigned char *newFileData = NULL;

    if (fileData != NULL)
    {
        if (dataSize <= (position*sizeof(int)))
        {
            // Increase data size up to position and store value
            newDataSize = (position + 1)*sizeof(int);
            newFileData = (unsigned char *)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL)
            {
                // RL_REALLOC succeded
                int *dataPtr = (int *)newFileData;
                dataPtr[position] = value;
            }
            else
            {
                // RL_REALLOC failed
                TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to realloc data (%u), position in bytes (%u) bigger than actual file size", STORAGE_DATA_FILE, dataSize, position*sizeof(int));

                // We store the old size of the file
                newFileData = fileData;
                newDataSize = dataSize;
            }
        }
        else
        {
            // Store the old size of the file
            newFileData = fileData;
            newDataSize = dataSize;

            // Replace value on selected position
            int *dataPtr = (int *)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(STORAGE_DATA_FILE, newFileData, newDataSize);
        RL_FREE(newFileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", STORAGE_DATA_FILE, value);
    }
    else
    {
        TraceLog(LOG_INFO, "FILEIO: [%s] File created successfully", STORAGE_DATA_FILE);

        dataSize = (position + 1)*sizeof(int);
        fileData = (unsigned char *)RL_MALLOC(dataSize);
        int *dataPtr = (int *)fileData;
        dataPtr[position] = value;

        success = SaveFileData(STORAGE_DATA_FILE, fileData, dataSize);
        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", STORAGE_DATA_FILE, value);
    }

    return success;
}

// Load integer value from storage file (from defined position)
// NOTE: If requested position could not be found, value 0 is returned
int LoadStorageValue(unsigned int position)
{
    int value = 0;
    unsigned int dataSize = 0;
    unsigned char *fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);

    if (fileData != NULL)
    {
        if (dataSize < (position*4)) TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to find storage position: %i", STORAGE_DATA_FILE, position);
        else
        {
            int *dataPtr = (int *)fileData;
            value = dataPtr[position];
        }

        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Loaded storage value: %i", STORAGE_DATA_FILE, value);
    }

    return value;
}

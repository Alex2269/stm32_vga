#include "Platform.h"
#include "xonix.h"

#include "startup.h"
#include "vga.h"
#include "gameFont4x4.h"
#include "Keyboard/ps2Keyboard.h"

#define LABEL_COLOR 0xFF00
#define SCORE_COLOR 0x3300

static uint8_t statusArea[H_STEPS * V_STEPS];

void DrawRunnerToGWorld()
{
    Vga::draw_text(gameFont4x4, gMyRunner.x * EATER_SIZE, gMyRunner.y * EATER_SIZE, "\x21");
}

void DrawWayToGWorld(int xPos, int yPos)
{
    Vga::draw_text(gameFont4x4, xPos * EATER_SIZE, yPos * EATER_SIZE, "\x25");
}

void DrawEaterToGWorld(int xPos, int yPos)
{
    Vga::draw_text(gameFont4x4, xPos * EATER_SIZE, yPos * EATER_SIZE, "\x22");
}

void DrawEmptyToGWorld(int xPos, int yPos)
{
    Vga::draw_text(gameFont4x4, xPos * EATER_SIZE, yPos * EATER_SIZE, " ");
}

void DrawFlyerToGWorld(int xPos, int yPos)
{
    Vga::draw_text(gameFont4x4, xPos * EATER_SIZE, yPos * EATER_SIZE, "\x23");
}

void DrawFilledToGWorld(int xPos, int yPos)
{
    Vga::draw_text(gameFont4x4, xPos * EATER_SIZE, yPos * EATER_SIZE, "\x24");
}

void DrawSmallFilledToGWorld(int xPos, int yPos)
{
    Vga::draw_text(gameFont4x4, xPos * EATER_SIZE, yPos * EATER_SIZE, "\x24");
}

void DrawComplete()
{
	int i, j;

	for (j = 0; j < V_STEPS; j++)
	{
		for (i = 0; i < H_STEPS; i++)
		{
			unsigned char c = *(gMyStatusArea + (j * H_STEPS) + i);
			if ((c & FILLED) || (c & BORDER) || (c & EATER))
			{
				DrawSmallFilledToGWorld(i, j);
			}
			else if (c & FLYER)
			{
				DrawFlyerToGWorld(i, j);
			}
			else if (c & WAY)
			{
				DrawWayToGWorld(i, j);
			}
			else if (c & RUNNER)
			{
				DrawRunnerToGWorld();
			}
			else
			{
				DrawEmptyToGWorld(i, j);
			}
		}
	}

	for (i = 0; i < gEaterCount; i++)
	{
		DrawEaterToGWorld(gEater[i].x, gEater[i].y);
	}
}

void DrawCompleteBorder()
{
	int i;

	for (i = 0; i < H_STEPS; i += RATIO)
	{
		DrawFilledToGWorld(i, 0);
		DrawFilledToGWorld(i, V_STEPS - 2);

		if (RATIO == 1)
		{
			DrawFilledToGWorld(i, 1);
			DrawFilledToGWorld(i, V_STEPS - 1);
		}
	}

	for (i = 2; i < V_STEPS - 2; i += RATIO)
	{
		DrawFilledToGWorld(0, i);
		DrawFilledToGWorld(H_STEPS - 1, i);

		if (RATIO == 1)
		{
			DrawFilledToGWorld(1, i);
			DrawFilledToGWorld(H_STEPS - 2, i);
		}
	}
}

void GameInit()
{
	//srand ((unsigned int) time (0));       /* Zufallsgenerator initialis.	*/

	gMyStatusArea = statusArea;

	Do_New();
}

int32_t GameUpdate()
{
	Animate();
    Vga::delay(STEP_TIME);

    int32_t scanCode = Ps2_GetScancode();
    switch (scanCode)
    {
    case KEY_SPACEBAR: // STOP
 		gMyRunner.dx = 0;
 		gMyRunner.dy = 0;
        break;
    case KEY_LEFTARROW:
 		gMyRunner.dx = -RATIO;
 		gMyRunner.dy = 0;
        break;
    case KEY_RIGHTARROW:
 		gMyRunner.dx = RATIO;
 		gMyRunner.dy = 0;
        break;
    case KEY_UPARROW:
 		gMyRunner.dy = -RATIO;
 		gMyRunner.dx = 0;
        break;
    case KEY_DOWNARROW:
 		gMyRunner.dy = RATIO;
 		gMyRunner.dx = 0;
        break;
    }

    return scanCode;
}

void Quit()
{
	ExitXonix(0);
}

void Pause()
{
}

void ScoreLevel(int num)
{
    Vga::printAt(0, 23, "Lvl:", LABEL_COLOR);

	char buffer[10];
	sprintf(buffer, "%02d ", num);
    Vga::printAt(4, 23, buffer, SCORE_COLOR);
}

void ScorePercentage(int num)
{
    Vga::printAt(7, 23, "Fl:", LABEL_COLOR);

	char buffer[10];
	sprintf(buffer, "%02d%% ", num);
    Vga::printAt(10, 23, buffer, SCORE_COLOR);
}

void ScoreRunner(int num)
{
    Vga::printAt(14, 23, "Pl:", LABEL_COLOR);

	char buffer[10];
	sprintf(buffer, "%02d ", num);
    Vga::printAt(17, 23, buffer, SCORE_COLOR);
}

void ScorePoints(int points)
{
    Vga::printAt(20, 23, "Score:", LABEL_COLOR);

	char buffer[20];
	sprintf(buffer, "%06d", points);
    Vga::printAt(26, 23, buffer, SCORE_COLOR);
}

void DisplayHighScore()
{
	// COORD dwCursorPosition;
	// DWORD nb;
	// dwCursorPosition.X = 45;
	// dwCursorPosition.Y = V_STEPS + 1;

	// char buffer[100];
	// sprintf(buffer, "HighScore: %d", gHighScore);

	// WriteConsoleOutputCharacterA(hStdOut, buffer, strlen(buffer), dwCursorPosition, &nb);
}

void ExitXonix(int status)
{
}

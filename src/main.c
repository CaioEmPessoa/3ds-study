
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

static int sqrSize = 15;

static int playerX;
static int playerY;

static int sqrsAmm = 0;
static int sqrsCoord[9999][2] = {};

int convertPos(char type, int pos)
{
	if (type == 'w') return (SCREEN_WIDTH/2)+pos;
	else if (type == 'h') return (SCREEN_HEIGHT/2)+pos;
	else return 0; // TODO if screen_heigh <= pos // pos = screen_height
}

void movePlayer(char nsew)
{
	switch (nsew)
	{
	case 'N':
		playerY -= 1;
		break;
	case 'S':
		playerY += 1;
		break;
	case 'E':
		playerX += 1;
		break;
	case 'W':
		playerX -= 1;
		break;
	default:
		break;
	}
}

// DRAWING FUNCTIONS
void addSquare()
{
	sqrsAmm += 1;
	sqrsCoord[sqrsAmm][0] = playerX;
	sqrsCoord[sqrsAmm][1] = playerY;
}
void eraseAll()
{
	sqrsAmm = 0;
	memset(sqrsCoord, 0, sizeof(sqrsCoord));
}

void checkKey(char key[])
{
	printf("\x1b[25;20H%s", key);

	if (strcmp(key, "KEY_DUP") == 0 || strcmp(key, "KEY_CPAD_UP") == 0)
	{
		printf("%s\x1b[20;20H", "CIMA");
		movePlayer('N');
	}
	else if (strcmp(key, "KEY_DDOWN") == 0 || strcmp(key, "KEY_CPAD_DOWN") == 0)
	{
		printf("%s\x1b[20;20H", "BAIXO");
		movePlayer('S');
	}
	else if (strcmp(key, "KEY_DRIGHT") == 0 || strcmp(key, "KEY_CPAD_RIGHT") == 0)
	{
		printf("%s\x1b[20;20H", "DIREITA");
		movePlayer('E');
	}
	else if (strcmp(key, "KEY_DLEFT") == 0 || strcmp(key, "KEY_CPAD_LEFT") == 0)
	{
		printf("%s\x1b[20;20H", "ESQUERDA");
		movePlayer('W');
	}
	else if(strcmp(key, "KEY_A") == 0)
	{
		addSquare();
	}
	else if(strcmp(key, "KEY_B") == 0)
	{
		eraseAll();
	}
}

int main(int argc, char* argv[]) 
{
	// Initialize services
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	gfxInitDefault();
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);
	
	// Initialize player position
	playerY = sqrSize / 2;
	playerX = sqrSize / 2;

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	char keysNames[32][32] = {
		"KEY_A", "KEY_B", "KEY_SELECT", "KEY_START",
		"KEY_DRIGHT", "KEY_DLEFT", "KEY_DUP", "KEY_DDOWN",
		"KEY_R", "KEY_L", "KEY_X", "KEY_Y",
		"", "", "KEY_ZL", "KEY_ZR",
		"", "", "", "",
		"KEY_TOUCH", "", "", "",
		"KEY_CSTICK_RIGHT", "KEY_CSTICK_LEFT", "KEY_CSTICK_UP", "KEY_CSTICK_DOWN",
		"KEY_CPAD_RIGHT", "KEY_CPAD_LEFT", "KEY_CPAD_UP", "KEY_CPAD_DOWN"
	};

	u32 clrGreen = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
	u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
	u32 clrClear = C2D_Color32(0xFF, 0xD8, 0xB0, 0x68);

	u32 kDownOld = 0, kHeldOld = 0;

	printf("\x1b[1;1HPress Start to exit.");

	// Main loop
	while (aptMainLoop())
	{
		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

		// draw objects
		C2D_DrawRectangle(
			convertPos('w', playerX), 
			convertPos('h', playerY), 
			0, sqrSize, sqrSize, 
			clrGreen, clrGreen, clrGreen, clrGreen
		);
		C2D_DrawTriangle(50 / 2, SCREEN_HEIGHT - 50, clrWhite,
			0,  SCREEN_HEIGHT, clrWhite,
			50, SCREEN_HEIGHT, clrWhite, 0);


		for (int i = sqrsAmm; i > 0; i--)
		{
			C2D_DrawRectangle(
				convertPos('w', sqrsCoord[i][0]),
				convertPos('h', sqrsCoord[i][1]),
				0, sqrSize, sqrSize, 
				clrGreen, clrGreen, clrGreen, clrGreen
			);
		}
		
		// end frame
		C3D_FrameEnd(0);

		//Scan all the inputs.y
		hidScanInput();

		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();

		if (kDown & KEY_START) break;

		if (kDown != kDownOld || kHeld == kHeldOld)
		{
			//Clear console
			consoleClear();
			// Line must be rewritten because we cleared the whole console
			printf("\x1b[1;1HPress Start to exit.");

			int i;
			for (i = 0; i < 32; i++)
			{
				if (kDown & BIT(i)) checkKey(keysNames[i]);
				if (kHeld & BIT(i)) checkKey(keysNames[i]);
			}
		}

		//Set keys old values for the next frame
		kDownOld = kDown;
		kHeldOld = kHeld;
	}

	// Exit services
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}

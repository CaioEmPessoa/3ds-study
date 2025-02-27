
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>

#define TOP_SCREEN_WIDTH  400
#define TOP_SCREEN_HEIGHT 240

#define BOT_SCREEN_WIDTH  320
#define BOT_SCREEN_HEIGHT 240

static int sqrSize = 15;

static int playerX;
static int playerY;

static int setColor = 2; // current color set of square and color that will be drawn

static int sqrsAmm = -1;
static int sqrsInfo[9999][3] = {};

int convertPos(char type, int pos)
{
	if (type == 'w') return (TOP_SCREEN_WIDTH/2)+pos;
	else if (type == 'h') return (TOP_SCREEN_HEIGHT/2)+pos;
	else return 0; // TODO if screen_heigh <= pos // pos = screen_height
}

void movePlayer(char nsew)
{
	switch (nsew)
	{
	case 'N':
		playerY -= 2;
		break;
	case 'S':
		playerY += 2;
		break;
	case 'E':
		playerX += 2;
		break;
	case 'W':
		playerX -= 2;
		break;
	default:
		break;
	}
}

// DRAWING FUNCTIONS
void addSquare()
{
	sqrsAmm += 1;
	sqrsInfo[sqrsAmm][0] = playerX; // x position
	sqrsInfo[sqrsAmm][1] = playerY; // y position
	sqrsInfo[sqrsAmm][2] = setColor; // square color
}
void changeColor(int dir)
{
	if (dir == 1 && setColor < 4)
	{
		setColor += 1;
	}
	else if (dir == 0 && setColor > 0)
	{
		setColor -= 1;
	}
}
void eraseAll()
{
	sqrsAmm = 0;
	memset(sqrsInfo, 0, sizeof(sqrsInfo));
}

void checkFrameKey(char key[]) // check in hold every frame
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
	else if(strcmp(key, "KEY_A") == 0) addSquare();
}

void checkSingleKey(char key[]) // check on diff click
{
	if(strcmp(key, "KEY_B") == 0) eraseAll();
	else if (strcmp(key, "KEY_R") == 0) changeColor(1);
	else if (strcmp(key, "KEY_L") == 0)changeColor(0);
}

// ------------| COLOR VARIABLES |-----------
u32 svdColors(int cId)
{
	u32 clrRed   = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
	u32 clrBlue  = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);
	u32 clrGreen = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
	u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
	
	u32 svdColors[4] = {
		clrRed, clrBlue, clrGreen, clrWhite
	};

	return svdColors[cId];
}

void drawSquare(int x, int y, int w, int h, int c)
{
	C2D_DrawRectangle(
		convertPos('w', x), 
		convertPos('h', y), 
		0, w, h,
		svdColors(c), svdColors(c), svdColors(c), svdColors(c)
	);
}

int main(int argc, char* argv[]) 
{
	// Initialize services
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	gfxInitDefault();
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	
	// Initialize player position
	playerY = sqrSize / 2;
	playerX = sqrSize / 2;

	// ARRAY WITH ALL KEY NAMES
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

	// BACKGROUND COLOR
	u32 clrClear = C2D_Color32(0xFF, 0xD8, 0xB0, 0x68);
	
	// to check if keys are pressed in the next frame
	u32 kDownOld = 0, kHeldOld = 0;

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bot = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	// printf("\x1b[1;1HPress Start to exit.");

	// Main loop
	while (aptMainLoop())
	{
		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_TargetClear(bot, clrClear);

		// draw objects ON TOP
		C2D_SceneBegin(top);
			// draw saved squares
			for (int i = 0; i < sqrsAmm - 1; i++)
			{
				drawSquare(sqrsInfo[i][0], sqrsInfo[i][1], 
					sqrSize, sqrSize, sqrsInfo[i][2]);
			}

			// draw cursor
			drawSquare(playerX, playerY, sqrSize, sqrSize, setColor);
		
		// draw objects on BOTTOM
		C2D_SceneBegin(bot);
			drawSquare(playerX, playerY, sqrSize, sqrSize, setColor);

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
				if (kDown & BIT(i)) checkSingleKey(keysNames[i]);
				if (kHeld & BIT(i)) checkFrameKey(keysNames[i]);
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
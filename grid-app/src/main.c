
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>

#define TOP_SCREEN_WIDTH  400
#define TOP_SCREEN_HEIGHT 240

#define BOT_SCREEN_WIDTH  320
#define BOT_SCREEN_HEIGHT 240

// board elements
static int maxTileX = 5; // set tile boundaries
static int maxTileY = 5;

// player elements
static int sqrSize = 20;

static int playerTileX = 0.5; // player positioning, starts on the middle
static int playerTileY = 0.5;

static bool cursorBot = false;

// screen that is being drawn on
static char slctScreen;

// touch elements
static touchPosition touch; // touch coords, touch.px & touch.py

// bottom screen touch elements
static int touchEl[50][4] = {}; // touch elements, the order will be stored on addTouchSquare or smth

int convertPos(char type, int pos)
{
	int offset = (slctScreen == 't') ? ((type == 'w') ? TOP_SCREEN_WIDTH / 2 : TOP_SCREEN_HEIGHT / 2) : 
										((type == 'w') ? BOT_SCREEN_WIDTH / 2 : BOT_SCREEN_HEIGHT / 2);

	return offset + pos;
}

void movePlayer(char nsew)
{
	switch (nsew)
	{
	case 'N':
		playerTileY -= 1;
		if (playerTileY <= -maxTileY-1) playerTileY += 1;
		break;
	case 'S':
		playerTileY += 1;
		if (playerTileY >= maxTileY) playerTileY -= 1;
		break;
	case 'E':
		playerTileX += 1;
		if (playerTileX >= maxTileX) playerTileX -= 1;
		break;
	case 'W':
		playerTileX -= 1;
		if (playerTileX <= -maxTileX-1) playerTileX += 1;
		break;
	default:
		break;
	}
}

bool checkTouchSquare(int id)
{
	int x0 = touchEl[id][0]; 
	int x1 = x0 + touchEl[id][2];
	int y0 = touchEl[id][1];
	int y1 = y0 + touchEl[id][3];

	int posX = touch.px - BOT_SCREEN_WIDTH / 2;
	int posY = touch.py - BOT_SCREEN_HEIGHT / 2;

	if ((posX >= x0 && posX <= x1) && (posY >= y0 && posY <= y1)) return true;
	else return false;
}

// id being manual is a choice, so that id's have known places on the array, and not 'random' ones
void addTouchSquare(int x, int y, int w, int h, int id)
{
	touchEl[id][0] = x;
	touchEl[id][1] = y;
	touchEl[id][2] = w;
	touchEl[id][3] = h;
}

void checkTouchPos()
{
	if (cursorBot)
	{ } // other thing to do with touch screen
	else
	{ } // utility functions
}

void checkFrameKey(char key[]) // check in hold every frame
{
	if (strcmp(key, "KEY_TOUCH") == 0) checkTouchPos();
}

void checkSingleKey(char key[]) // check on diff click
{
	if (strcmp(key, "KEY_Y") == 0) cursorBot = cursorBot ? false : true;
	else if (strcmp(key, "KEY_DUP") == 0 || strcmp(key, "KEY_CPAD_UP") == 0) movePlayer('N');
	else if (strcmp(key, "KEY_DDOWN") == 0 || strcmp(key, "KEY_CPAD_DOWN") == 0) movePlayer('S');
	else if (strcmp(key, "KEY_DRIGHT") == 0 || strcmp(key, "KEY_CPAD_RIGHT") == 0) movePlayer('E');
	else if (strcmp(key, "KEY_DLEFT") == 0 || strcmp(key, "KEY_CPAD_LEFT") == 0) movePlayer('W');
}

// ------------| COLOR VARIABLES |-----------
u32 svdColors(int cId)
{
	u32 clrWhite  = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
	u32 clrBlack  = C2D_Color32(0x00, 0x00, 0x00, 0xFF);
	u32 clrRed    = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
	u32 clrBlue   = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);
	u32 clrGreen  = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
	u32 clrPink   = C2D_Color32(0xFF, 0xC0, 0xCB, 0xFF);
	u32 clrOrange = C2D_Color32(0xFF, 0xA5, 0x00, 0xFF);
	u32 clrPurple = C2D_Color32(0x80, 0x00, 0x80, 0xFF);
	u32 clrClear = C2D_Color32(0xFF, 0xD8, 0xB0, 0xFF);

	u32 svdColors[9] = {
		clrRed, clrBlue, clrGreen, clrWhite, clrPink, clrOrange, clrPurple, clrBlack, clrClear
	};

	return svdColors[cId];
}

// xy positions for square // width&height of square 
// color code in list 	  // bool r (unused) if xy are relative to middle or top-left
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
	
	// -------| INITIAL VARIABLES |------

	// BACKGROUND POSITION
	int backgroundSizeX = (maxTileX*sqrSize)*2;
	int backgroundSizeY = (maxTileY*sqrSize)*2;

	// ARRAY WITH ALL KEY
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
	int posXOld = 0, posYOld = 0;

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

		// Utilitys Screen (mainly bottom)
		slctScreen =  cursorBot ? 't' : 'b';
		C2D_SceneBegin(cursorBot ? top : bot);

			// for (int i = 7; i < 11; i++) {
			// 	int posIndex = (i - 3) % 4;
			// 	drawSquare(50, positions[posIndex], sqrSize, sqrSize, i-3);
			// 	addTouchSquare(50-3, positions[posIndex]-3, sqrSize+6, sqrSize+6, i); // +5 offset
			// }

		// Drawing Canvas (mainly top screen)
		slctScreen =  cursorBot ? 'b' : 't';
		C2D_SceneBegin(cursorBot ? bot : top);

			drawSquare(-backgroundSizeX*0.5, -backgroundSizeY*0.5, backgroundSizeX, backgroundSizeY, 5);

			drawSquare(playerTileX*sqrSize, playerTileY*sqrSize, sqrSize, sqrSize, 6);

		// end frame
		C3D_FrameEnd(0);

		//Read the touch screen coordinates
		hidTouchRead(&touch);
		//Scan all the inputs
		hidScanInput();

		u32 kDown = hidKeysDown(), kHeld = hidKeysHeld();

		if (kDown & KEY_START) break;
		
		if (kDown != kDownOld || kHeld == kHeldOld || 
			posXOld != touch.px || posYOld != touch.py)
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
		posXOld = touch.px;
		posYOld = touch.py;
	}

	// Exit services
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}
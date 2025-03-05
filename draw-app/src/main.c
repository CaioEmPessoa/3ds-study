
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>

#define TOP_SCREEN_WIDTH  400
#define TOP_SCREEN_HEIGHT 240

#define BOT_SCREEN_WIDTH  320
#define BOT_SCREEN_HEIGHT 240

static int sqrSizes[3] = {10, 15, 20};
static int sqrSize;

// player elements
static int sqrSize = 15;

static int playerX;
static int playerY;

static int setColor = 2; // current color set of square and color that will be drawn

// drawing elements
static int sqrsAmm = -1;
static int sqrsInfo[9999][4] = {};

static bool drawBot = false;

// screen that is being drawn on
static char slctScreen;

// touch elements
static touchPosition touch; // touch coords, touch.px & touch.py

// bottom screen touch elements
static int touchEl[9999][4] = {}; // touch elements, the order will be stored on addTouchSquare or smth

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
	sqrsInfo[sqrsAmm][2]= sqrSize; // square size
	sqrsInfo[sqrsAmm][3] = setColor; // square color
}

void addSquareTouch(int x, int y)
{
	sqrsAmm += 1;
	sqrsInfo[sqrsAmm][0] = x - BOT_SCREEN_WIDTH / 2;
	sqrsInfo[sqrsAmm][1] = y  - BOT_SCREEN_HEIGHT / 2;
	sqrsInfo[sqrsAmm][2]= sqrSize; // square size
	sqrsInfo[sqrsAmm][3] = setColor; // square color
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
	sqrsAmm = -1;
	memset(sqrsInfo, 0, sizeof(sqrsInfo));
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
	if (drawBot == true) addSquareTouch(touch.px, touch.py);
	else
	{
		// change sqr size
		if (checkTouchSquare(0)) sqrSize = sqrSizes[0]; // grande
		else if (checkTouchSquare(1)) sqrSize = sqrSizes[1]; // médio
		else if (checkTouchSquare(2)) sqrSize = sqrSizes[2]; // pequeno

		// change colors
		else if (checkTouchSquare(3)) setColor = 0; // red
		else if (checkTouchSquare(4)) setColor = 1; // blue
		else if (checkTouchSquare(5)) setColor = 2; // green
		else if (checkTouchSquare(6)) setColor = 3; // white
	}
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
	else if (strcmp(key, "KEY_TOUCH") == 0) checkTouchPos();
}

void checkSingleKey(char key[]) // check on diff click
{
	if(strcmp(key, "KEY_B") == 0) eraseAll();
	else if (strcmp(key, "KEY_R") == 0) changeColor(1);
	else if (strcmp(key, "KEY_L") == 0) changeColor(0);
	else if (strcmp(key, "KEY_Y") == 0) drawBot = drawBot ? false : true;
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
	sqrSize = sqrSizes[0];

	// INITIAL PLAYER POSITION
	playerY = sqrSize / 2;
	playerX = sqrSize / 2;

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
	int posXOld = 0;
	int posYOld = 0;

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
		
		// Drawing Canvas (mainly top screen)
		slctScreen =  drawBot ? 'b' : 't';
		C2D_SceneBegin(drawBot ? bot : top);
			// draw saved squares
			for (int i = 0; i < sqrsAmm - 1; i++)
			{
				drawSquare(sqrsInfo[i][0], sqrsInfo[i][1], 
					sqrsInfo[i][2], sqrsInfo[i][2], sqrsInfo[i][3]);
			}

			// draw cursor
			drawSquare(playerX, playerY, sqrSize, sqrSize, setColor);

		// Utilitys Screen (mainly bottom)
		slctScreen =  drawBot ? 't' : 'b';
		C2D_SceneBegin(drawBot ? top : bot);

			// Add change square size graphics and hitboxes
			for (int i = 0; i < 3; i++) {
				drawSquare(-100 + sqrSizes[i] / 2, -60 + i * 60, sqrSizes[i], sqrSizes[i], setColor);
				addTouchSquare(-100 + sqrSizes[i] / 2, -60 + i * 60, sqrSizes[i], sqrSizes[i], i); 
			}

			// add change square color graphics and hitboxes
			for (int i = 3; i < 7; i++) {
				drawSquare(0, -60 + (i-3) * 60, sqrSize, sqrSize, i-3);
				addTouchSquare(0, -60 + (i-3) * 60, sqrSize+5, sqrSize+5, i); // +5 offset
			}

		// end frame
		C3D_FrameEnd(0);

		//Read the touch screen coordinates
		hidTouchRead(&touch);
		//Scan all the inputs
		hidScanInput();

		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();

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
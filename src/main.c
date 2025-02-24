
#include <3ds.h>
#include <stdio.h>

void checkKey(char key[])
{
	printf("\x1b[25;25H%s", key);
}

int main(int argc, char **argv)
{
	//Matrix containing the name of each key. Useful for printing when a key is pressed
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

	// Initialize services
	gfxInitDefault();

	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	consoleInit(GFX_TOP, NULL);

	u32 kDownOld = 0, kHeldOld = 0, kUpOld = 0; //In these variables there will be information about keys detected in the previous frame

	printf("\x1b[1;1HPress Start to exit.");

	// Main loop
	while (aptMainLoop())
	{
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();
		//hidKeysHeld returns information about which buttons have are held down in this frame
		u32 kHeld = hidKeysHeld();
		//hidKeysUp returns information about which buttons have been just released
		u32 kUp = hidKeysUp();

		if (kDown & KEY_START) break; // break in order to return to hbmenu

		//Do the keys printing only if keys have changed
		if (kDown != kDownOld || kHeld != kHeldOld)
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
		kUpOld = kUp;

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}

	// Exit services
	gfxExit();
	return 0;
}

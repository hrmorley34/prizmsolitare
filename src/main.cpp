#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/system.h>
#include <stdlib.h>
#include <time.h>
#include "game.hpp"
#include "controls.hpp"
#include "save.hpp"
#include "imageclasses.hpp"

void setup()
{
    VRAM = (color_t *)GetVRAMAddress();

    Bdisp_EnableColor(1);
    Bkey_SetAllFlags(0x80);    // disable shift+4 for catalog
    EnableDisplayHeader(0, 0); // disable status area
}

#define GetInputMode() GetSetupSetting(20)
#define SetInputMode(value) SetSetupSetting(20, value)

Game g;
Controls c = Controls(&g);

void ResetGame()
{
    g = Game();
    c = Controls(&g);
}

bool doSaveState;
void OnClose()
{
    if (doSaveState)
        SaveState(&g);
}

int main(void)
{
    int key;
    unsigned char lastKeyModState = GetInputMode();
    unsigned char currentKeyModState;

    setup();

    sys_srand(clock());
    ResetGame();
    ReadState(&g);
    SetQuitHandler(OnClose);
    doSaveState = true;

    Bdisp_AllClr_VRAM();
    c.ClearScreen();
    c.DrawScreen();

    while (true)
    {
        GetKey(&key);

        if (key == KEY_CTRL_QUIT)
        {
            // Quit deletes the save file and resets the game, then triggers the MENU key
            doSaveState = false;
            DeleteState();
            ResetGame();
            c.ClearScreen();
            c.DrawScreen();

            Keyboard_PutKeycode(4, 9, 0);
            GetKey(&key);

            lastKeyModState = 255; // force re-setting
            doSaveState = true;
        }

        // Paint over header when modifiers aren't active
        currentKeyModState = GetInputMode();
        if ((currentKeyModState & 0x80) != 0)
        {
            // currently set to alpha lock. Unset this
            SetInputMode(currentKeyModState = 0);
            // force redraw
            lastKeyModState = 255;
        }
        if (currentKeyModState != lastKeyModState)
        {
            lastKeyModState = currentKeyModState;
            if (lastKeyModState == 0)
            {
                EnableDisplayHeader(0, 0);
                // c.ClearScreen();
            }
            else
            {
                EnableDisplayHeader(2, 1);
            }
        }

        switch (key)
        {
        case KEY_CTRL_MENU:
            setup();
            break;
        case KEY_CTRL_AC:
            ResetGame();
            break;
        }
        c.ClearScreen();
        c.HandleKey(key);
        // c.DrawScreen();
    }
}

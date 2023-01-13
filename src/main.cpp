#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/system.h>
#include <stdlib.h>
#include <time.h>
#include "game.hpp"
#include "controls.hpp"
#include "save.hpp"

void setup()
{
    VRAM = (color_t *)GetVRAMAddress();

    Bdisp_EnableColor(1);
    Bkey_SetAllFlags(0x80);    // disable shift+4 for catalog
    EnableDisplayHeader(0, 0); // disable status area
}

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
        switch (key)
        {
        case KEY_CTRL_MENU:
            setup();
            break;
        case KEY_CTRL_AC:
            ResetGame();
            break;
        }
        c.HandleKey(key);
        // c.DrawScreen();
    }
}

#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <stdlib.h>
#include <time.h>
#include "game.hpp"
#include "controls.hpp"

void setup()
{
    VRAM = (color_t *)GetVRAMAddress();

    Bdisp_EnableColor(1);
    Bkey_SetAllFlags(0x80);    // disable shift+4 for catalog
    EnableDisplayHeader(0, 0); // disable status area
    FrameColor(1, BackgroundColour);
}

int main(void)
{
    int key;

    setup();

    sys_srand(clock());
    DefineCardDeck();
    Game g = Game();
    Controls c = Controls(&g);

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
            g = Game();
            c = Controls(&g);
            break;
        }
        c.HandleKey(key);
        // c.DrawScreen();
    }
}

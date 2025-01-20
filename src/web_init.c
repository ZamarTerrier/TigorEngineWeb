#include "window_manager.h"

extern wManagerWindow _wMWindow;

// Create key code translation tables
//
void createKeyTables(void *wData)
{
    int scancode;

    wManagerWeb* windowData = wData;

    memset(windowData->keycodes, -1, sizeof(windowData->keycodes));
    memset(windowData->scancodes, -1, sizeof(windowData->scancodes));

    windowData->keycodes[SDL_SCANCODE_0] = TIGOR_KEY_0;
    windowData->keycodes[SDL_SCANCODE_1] = TIGOR_KEY_1;
    windowData->keycodes[SDL_SCANCODE_2] = TIGOR_KEY_2;
    windowData->keycodes[SDL_SCANCODE_3] = TIGOR_KEY_3;
    windowData->keycodes[SDL_SCANCODE_4] = TIGOR_KEY_4;
    windowData->keycodes[SDL_SCANCODE_5] = TIGOR_KEY_5;
    windowData->keycodes[SDL_SCANCODE_6] = TIGOR_KEY_6;
    windowData->keycodes[SDL_SCANCODE_7] = TIGOR_KEY_7;
    windowData->keycodes[SDL_SCANCODE_8] = TIGOR_KEY_8;
    windowData->keycodes[SDL_SCANCODE_9] = TIGOR_KEY_9;
    windowData->keycodes[SDL_SCANCODE_A] = TIGOR_KEY_A;
    windowData->keycodes[SDL_SCANCODE_B] = TIGOR_KEY_B;
    windowData->keycodes[SDL_SCANCODE_C] = TIGOR_KEY_C;
    windowData->keycodes[SDL_SCANCODE_D] = TIGOR_KEY_D;
    windowData->keycodes[SDL_SCANCODE_E] = TIGOR_KEY_E;
    windowData->keycodes[SDL_SCANCODE_F] = TIGOR_KEY_F;
    windowData->keycodes[SDL_SCANCODE_G] = TIGOR_KEY_G;
    windowData->keycodes[SDL_SCANCODE_H] = TIGOR_KEY_H;
    windowData->keycodes[SDL_SCANCODE_I] = TIGOR_KEY_I;
    windowData->keycodes[SDL_SCANCODE_J] = TIGOR_KEY_J;
    windowData->keycodes[SDL_SCANCODE_K] = TIGOR_KEY_K;
    windowData->keycodes[SDL_SCANCODE_L] = TIGOR_KEY_L;
    windowData->keycodes[SDL_SCANCODE_M] = TIGOR_KEY_M;
    windowData->keycodes[SDL_SCANCODE_N] = TIGOR_KEY_N;
    windowData->keycodes[SDL_SCANCODE_O] = TIGOR_KEY_O;
    windowData->keycodes[SDL_SCANCODE_P] = TIGOR_KEY_P;
    windowData->keycodes[SDL_SCANCODE_Q] = TIGOR_KEY_Q;
    windowData->keycodes[SDL_SCANCODE_R] = TIGOR_KEY_R;
    windowData->keycodes[SDL_SCANCODE_S] = TIGOR_KEY_S;
    windowData->keycodes[SDL_SCANCODE_T] = TIGOR_KEY_T;
    windowData->keycodes[SDL_SCANCODE_U] = TIGOR_KEY_U;
    windowData->keycodes[SDL_SCANCODE_V] = TIGOR_KEY_V;
    windowData->keycodes[SDL_SCANCODE_W] = TIGOR_KEY_W;
    windowData->keycodes[SDL_SCANCODE_X] = TIGOR_KEY_X;
    windowData->keycodes[SDL_SCANCODE_Y] = TIGOR_KEY_Y;
    windowData->keycodes[SDL_SCANCODE_Z] = TIGOR_KEY_Z;

    windowData->keycodes[SDL_SCANCODE_APOSTROPHE] = TIGOR_KEY_APOSTROPHE;
    windowData->keycodes[SDL_SCANCODE_BACKSLASH] = TIGOR_KEY_BACKSLASH;
    windowData->keycodes[SDL_SCANCODE_COMMA] = TIGOR_KEY_COMMA;
    windowData->keycodes[SDL_SCANCODE_EQUALS] = TIGOR_KEY_EQUAL;
    windowData->keycodes[SDL_SCANCODE_GRAVE] = TIGOR_KEY_GRAVE_ACCENT;
    windowData->keycodes[SDL_SCANCODE_LEFTBRACKET] = TIGOR_KEY_LEFT_BRACKET;
    windowData->keycodes[SDL_SCANCODE_MINUS] = TIGOR_KEY_MINUS;
    windowData->keycodes[SDL_SCANCODE_PERIOD] = TIGOR_KEY_PERIOD;
    windowData->keycodes[SDL_SCANCODE_RIGHTBRACKET] = TIGOR_KEY_RIGHT_BRACKET;
    windowData->keycodes[SDL_SCANCODE_SEMICOLON] = TIGOR_KEY_SEMICOLON;
    windowData->keycodes[SDL_SCANCODE_SLASH] = TIGOR_KEY_SLASH;
    //windowData->keycodes[0x056] = TIGOR_KEY_WORLD_2;

    windowData->keycodes[SDL_SCANCODE_BACKSPACE] = TIGOR_KEY_BACKSPACE;
    windowData->keycodes[SDL_SCANCODE_DELETE] = TIGOR_KEY_DELETE;
    windowData->keycodes[SDL_SCANCODE_END] = TIGOR_KEY_END;
    windowData->keycodes[SDL_SCANCODE_RETURN] = TIGOR_KEY_ENTER;
    windowData->keycodes[SDL_SCANCODE_ESCAPE] = TIGOR_KEY_ESCAPE;
    windowData->keycodes[SDL_SCANCODE_HOME] = TIGOR_KEY_HOME;
    windowData->keycodes[SDL_SCANCODE_INSERT] = TIGOR_KEY_INSERT;
    windowData->keycodes[SDL_SCANCODE_MENU] = TIGOR_KEY_MENU;
    windowData->keycodes[SDL_SCANCODE_PAGEDOWN] = TIGOR_KEY_PAGE_DOWN;
    windowData->keycodes[SDL_SCANCODE_PAGEUP] = TIGOR_KEY_PAGE_UP;
    windowData->keycodes[SDL_SCANCODE_PAUSE] = TIGOR_KEY_PAUSE;
    windowData->keycodes[SDL_SCANCODE_SPACE] = TIGOR_KEY_SPACE;
    windowData->keycodes[SDL_SCANCODE_TAB] = TIGOR_KEY_TAB;
    windowData->keycodes[SDL_SCANCODE_CAPSLOCK] = TIGOR_KEY_CAPS_LOCK;
    windowData->keycodes[SDL_SCANCODE_NUMLOCKCLEAR] = TIGOR_KEY_NUM_LOCK;
    windowData->keycodes[SDL_SCANCODE_SCROLLLOCK] = TIGOR_KEY_SCROLL_LOCK;
    windowData->keycodes[SDL_SCANCODE_F1 ] = TIGOR_KEY_F1;
    windowData->keycodes[SDL_SCANCODE_F2 ] = TIGOR_KEY_F2;
    windowData->keycodes[SDL_SCANCODE_F3 ] = TIGOR_KEY_F3;
    windowData->keycodes[SDL_SCANCODE_F4 ] = TIGOR_KEY_F4;
    windowData->keycodes[SDL_SCANCODE_F5 ] = TIGOR_KEY_F5;
    windowData->keycodes[SDL_SCANCODE_F6 ] = TIGOR_KEY_F6;
    windowData->keycodes[SDL_SCANCODE_F7 ] = TIGOR_KEY_F7;
    windowData->keycodes[SDL_SCANCODE_F8 ] = TIGOR_KEY_F8;
    windowData->keycodes[SDL_SCANCODE_F9 ] = TIGOR_KEY_F9;
    windowData->keycodes[SDL_SCANCODE_F10] = TIGOR_KEY_F10;
    windowData->keycodes[SDL_SCANCODE_F11] = TIGOR_KEY_F11;
    windowData->keycodes[SDL_SCANCODE_F12] = TIGOR_KEY_F12;
    /*windowData->keycodes[0x064] = TIGOR_KEY_F13;
    windowData->keycodes[0x065] = TIGOR_KEY_F14;
    windowData->keycodes[0x066] = TIGOR_KEY_F15;
    windowData->keycodes[0x067] = TIGOR_KEY_F16;
    windowData->keycodes[0x068] = TIGOR_KEY_F17;
    windowData->keycodes[0x069] = TIGOR_KEY_F18;
    windowData->keycodes[0x06A] = TIGOR_KEY_F19;
    windowData->keycodes[0x06B] = TIGOR_KEY_F20;
    windowData->keycodes[0x06C] = TIGOR_KEY_F21;
    windowData->keycodes[0x06D] = TIGOR_KEY_F22;
    windowData->keycodes[0x06E] = TIGOR_KEY_F23;
    windowData->keycodes[0x076] = TIGOR_KEY_F24;*/
    windowData->keycodes[SDL_SCANCODE_LALT] = TIGOR_KEY_LEFT_ALT;
    windowData->keycodes[SDL_SCANCODE_LCTRL] = TIGOR_KEY_LEFT_CONTROL;
    windowData->keycodes[SDL_SCANCODE_LSHIFT] = TIGOR_KEY_LEFT_SHIFT;
    windowData->keycodes[SDL_SCANCODE_LGUI] = TIGOR_KEY_LEFT_SUPER;
    windowData->keycodes[SDL_SCANCODE_PRINTSCREEN] = TIGOR_KEY_PRINT_SCREEN;
    windowData->keycodes[SDL_SCANCODE_RALT] = TIGOR_KEY_RIGHT_ALT;
    windowData->keycodes[SDL_SCANCODE_RCTRL] = TIGOR_KEY_RIGHT_CONTROL;
    windowData->keycodes[SDL_SCANCODE_RSHIFT] = TIGOR_KEY_RIGHT_SHIFT;
    windowData->keycodes[SDL_SCANCODE_RGUI] = TIGOR_KEY_RIGHT_SUPER;
    windowData->keycodes[SDL_SCANCODE_DOWN] = TIGOR_KEY_DOWN;
    windowData->keycodes[SDL_SCANCODE_LEFT] = TIGOR_KEY_LEFT;
    windowData->keycodes[SDL_SCANCODE_RIGHT] = TIGOR_KEY_RIGHT;
    windowData->keycodes[SDL_SCANCODE_UP] = TIGOR_KEY_UP;

    windowData->keycodes[SDL_SCANCODE_KP_0] = TIGOR_KEY_KP_0;
    windowData->keycodes[SDL_SCANCODE_KP_1] = TIGOR_KEY_KP_1;
    windowData->keycodes[SDL_SCANCODE_KP_2] = TIGOR_KEY_KP_2;
    windowData->keycodes[SDL_SCANCODE_KP_3] = TIGOR_KEY_KP_3;
    windowData->keycodes[SDL_SCANCODE_KP_4] = TIGOR_KEY_KP_4;
    windowData->keycodes[SDL_SCANCODE_KP_5] = TIGOR_KEY_KP_5;
    windowData->keycodes[SDL_SCANCODE_KP_6] = TIGOR_KEY_KP_6;
    windowData->keycodes[SDL_SCANCODE_KP_7] = TIGOR_KEY_KP_7;
    windowData->keycodes[SDL_SCANCODE_KP_8] = TIGOR_KEY_KP_8;
    windowData->keycodes[SDL_SCANCODE_KP_9] = TIGOR_KEY_KP_9;
    windowData->keycodes[SDL_SCANCODE_KP_PLUS] = TIGOR_KEY_KP_ADD;
    windowData->keycodes[SDL_SCANCODE_KP_DECIMAL] = TIGOR_KEY_KP_DECIMAL;
    windowData->keycodes[SDL_SCANCODE_KP_DIVIDE] = TIGOR_KEY_KP_DIVIDE;
    windowData->keycodes[SDL_SCANCODE_KP_ENTER] = TIGOR_KEY_KP_ENTER;
    windowData->keycodes[SDL_SCANCODE_KP_EQUALS] = TIGOR_KEY_KP_EQUAL;
    windowData->keycodes[SDL_SCANCODE_KP_MULTIPLY] = TIGOR_KEY_KP_MULTIPLY;
    windowData->keycodes[SDL_SCANCODE_KP_MINUS] = TIGOR_KEY_KP_SUBTRACT;

    for (scancode = 0;  scancode < 512;  scancode++)
    {
        if (windowData->keycodes[scancode] > 0)
            windowData->scancodes[windowData->keycodes[scancode]] = scancode;
    }
}

void _wManagerUpdateKeyNamesWeb()
{
    int key;
    char state[256] = {0};

    memset(((wManagerWeb* )_wMWindow.WindowData)->keynames, 0, sizeof(((wManagerWeb* )_wMWindow.WindowData)->keynames));

}


uint32_t _wManagerInitWeb(void)
{
    _wManagerUpdateKeyNamesWeb();

    return 1;
}

void _wManagerTerminateWeb(void){

}

extern void _wManagerGetCursorPosWeb(wManagerWindow *window, double* xpos, double* ypos);
extern void _wManagerSetCursorPosWeb(wManagerWindow *window, double xpos, double ypos);
extern const char* _wManagerGetScancodeNameWeb(int scancode);
extern int _wManagerGetKeyScancodeWeb(int key);

int _wManagerConnectWeb(_wManagerPlatform* platform){
    
    const _wManagerPlatform web =
    {
        .init = _wManagerInitWeb,
        .terminate = _wManagerTerminateWeb,
        .getCursorPos = _wManagerGetCursorPosWeb,
        .setCursorPos = _wManagerSetCursorPosWeb,
        .getScancodeName = _wManagerGetScancodeNameWeb,
        .getKeyScancode = _wManagerGetKeyScancodeWeb
    };

    *platform = web;

    return true;
}
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

    windowData->keycodes[SDLK_0] = TIGOR_KEY_0;
    windowData->keycodes[SDLK_1] = TIGOR_KEY_1;
    windowData->keycodes[SDLK_2] = TIGOR_KEY_2;
    windowData->keycodes[SDLK_3] = TIGOR_KEY_3;
    windowData->keycodes[SDLK_4] = TIGOR_KEY_4;
    windowData->keycodes[SDLK_5] = TIGOR_KEY_5;
    windowData->keycodes[SDLK_6] = TIGOR_KEY_6;
    windowData->keycodes[SDLK_7] = TIGOR_KEY_7;
    windowData->keycodes[SDLK_8] = TIGOR_KEY_8;
    windowData->keycodes[SDLK_9] = TIGOR_KEY_9;
    windowData->keycodes[SDLK_a] = TIGOR_KEY_A;
    windowData->keycodes[SDLK_b] = TIGOR_KEY_B;
    windowData->keycodes[SDLK_c] = TIGOR_KEY_C;
    windowData->keycodes[SDLK_d] = TIGOR_KEY_D;
    windowData->keycodes[SDLK_e] = TIGOR_KEY_E;
    windowData->keycodes[SDLK_f] = TIGOR_KEY_F;
    windowData->keycodes[SDLK_g] = TIGOR_KEY_G;
    windowData->keycodes[SDLK_h] = TIGOR_KEY_H;
    windowData->keycodes[SDLK_i] = TIGOR_KEY_I;
    windowData->keycodes[SDLK_j] = TIGOR_KEY_J;
    windowData->keycodes[SDLK_k] = TIGOR_KEY_K;
    windowData->keycodes[SDLK_l] = TIGOR_KEY_L;
    windowData->keycodes[SDLK_m] = TIGOR_KEY_M;
    windowData->keycodes[SDLK_n] = TIGOR_KEY_N;
    windowData->keycodes[SDLK_o] = TIGOR_KEY_O;
    windowData->keycodes[SDLK_p] = TIGOR_KEY_P;
    windowData->keycodes[SDLK_q] = TIGOR_KEY_Q;
    windowData->keycodes[SDLK_r] = TIGOR_KEY_R;
    windowData->keycodes[SDLK_s] = TIGOR_KEY_S;
    windowData->keycodes[SDLK_t] = TIGOR_KEY_T;
    windowData->keycodes[SDLK_u] = TIGOR_KEY_U;
    windowData->keycodes[SDLK_v] = TIGOR_KEY_V;
    windowData->keycodes[SDLK_w] = TIGOR_KEY_W;
    windowData->keycodes[SDLK_x] = TIGOR_KEY_X;
    windowData->keycodes[SDLK_y] = TIGOR_KEY_Y;
    windowData->keycodes[SDLK_z] = TIGOR_KEY_Z;

    windowData->keycodes[SDLK_ASTERISK] = TIGOR_KEY_APOSTROPHE;
    windowData->keycodes[SDLK_BACKSLASH] = TIGOR_KEY_BACKSLASH;
    windowData->keycodes[SDLK_COMMA] = TIGOR_KEY_COMMA;
    windowData->keycodes[SDLK_EQUALS] = TIGOR_KEY_EQUAL;
    windowData->keycodes[SDLK_BACKQUOTE] = TIGOR_KEY_GRAVE_ACCENT;
    windowData->keycodes[SDLK_LEFTBRACKET] = TIGOR_KEY_LEFT_BRACKET;
    windowData->keycodes[SDLK_MINUS] = TIGOR_KEY_MINUS;
    windowData->keycodes[SDLK_PERIOD] = TIGOR_KEY_PERIOD;
    windowData->keycodes[SDLK_RIGHTBRACKET] = TIGOR_KEY_RIGHT_BRACKET;
    windowData->keycodes[SDLK_SEMICOLON] = TIGOR_KEY_SEMICOLON;
    windowData->keycodes[SDLK_SLASH] = TIGOR_KEY_SLASH;
    //windowData->keycodes[0x056] = TIGOR_KEY_WORLD_2;

    windowData->keycodes[SDLK_BACKSPACE] = TIGOR_KEY_BACKSPACE;
    windowData->keycodes[SDLK_DELETE] = TIGOR_KEY_DELETE;
    windowData->keycodes[SDLK_END] = TIGOR_KEY_END;
    windowData->keycodes[SDLK_RETURN] = TIGOR_KEY_ENTER;
    windowData->keycodes[SDLK_ESCAPE] = TIGOR_KEY_ESCAPE;
    windowData->keycodes[SDLK_HOME] = TIGOR_KEY_HOME;
    windowData->keycodes[SDLK_INSERT] = TIGOR_KEY_INSERT;
    windowData->keycodes[SDLK_MENU] = TIGOR_KEY_MENU;
    windowData->keycodes[SDLK_PAGEDOWN] = TIGOR_KEY_PAGE_DOWN;
    windowData->keycodes[SDLK_PAGEUP] = TIGOR_KEY_PAGE_UP;
    windowData->keycodes[SDLK_PAUSE] = TIGOR_KEY_PAUSE;
    windowData->keycodes[SDLK_SPACE] = TIGOR_KEY_SPACE;
    windowData->keycodes[SDLK_TAB] = TIGOR_KEY_TAB;
    windowData->keycodes[SDLK_CAPSLOCK] = TIGOR_KEY_CAPS_LOCK;
    windowData->keycodes[SDLK_NUMLOCKCLEAR] = TIGOR_KEY_NUM_LOCK;
    windowData->keycodes[SDLK_SCROLLLOCK] = TIGOR_KEY_SCROLL_LOCK;
    windowData->keycodes[SDLK_F1 ] = TIGOR_KEY_F1;
    windowData->keycodes[SDLK_F2 ] = TIGOR_KEY_F2;
    windowData->keycodes[SDLK_F3 ] = TIGOR_KEY_F3;
    windowData->keycodes[SDLK_F4 ] = TIGOR_KEY_F4;
    windowData->keycodes[SDLK_F5 ] = TIGOR_KEY_F5;
    windowData->keycodes[SDLK_F6 ] = TIGOR_KEY_F6;
    windowData->keycodes[SDLK_F7 ] = TIGOR_KEY_F7;
    windowData->keycodes[SDLK_F8 ] = TIGOR_KEY_F8;
    windowData->keycodes[SDLK_F9 ] = TIGOR_KEY_F9;
    windowData->keycodes[SDLK_F10] = TIGOR_KEY_F10;
    windowData->keycodes[SDLK_F11] = TIGOR_KEY_F11;
    windowData->keycodes[SDLK_F12] = TIGOR_KEY_F12;
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
    windowData->keycodes[SDLK_LALT] = TIGOR_KEY_LEFT_ALT;
    windowData->keycodes[SDLK_LCTRL] = TIGOR_KEY_LEFT_CONTROL;
    windowData->keycodes[SDLK_LSHIFT] = TIGOR_KEY_LEFT_SHIFT;
    windowData->keycodes[SDLK_LGUI] = TIGOR_KEY_LEFT_SUPER;
    windowData->keycodes[SDLK_PRINTSCREEN] = TIGOR_KEY_PRINT_SCREEN;
    windowData->keycodes[SDLK_RALT] = TIGOR_KEY_RIGHT_ALT;
    windowData->keycodes[SDLK_RCTRL] = TIGOR_KEY_RIGHT_CONTROL;
    windowData->keycodes[SDLK_RSHIFT] = TIGOR_KEY_RIGHT_SHIFT;
    windowData->keycodes[SDLK_RGUI] = TIGOR_KEY_RIGHT_SUPER;
    windowData->keycodes[SDLK_DOWN] = TIGOR_KEY_DOWN;
    windowData->keycodes[SDLK_LEFT] = TIGOR_KEY_LEFT;
    windowData->keycodes[SDLK_RIGHT] = TIGOR_KEY_RIGHT;
    windowData->keycodes[SDLK_UP] = TIGOR_KEY_UP;

    windowData->keycodes[SDLK_KP_0] = TIGOR_KEY_KP_0;
    windowData->keycodes[SDLK_KP_1] = TIGOR_KEY_KP_1;
    windowData->keycodes[SDLK_KP_2] = TIGOR_KEY_KP_2;
    windowData->keycodes[SDLK_KP_3] = TIGOR_KEY_KP_3;
    windowData->keycodes[SDLK_KP_4] = TIGOR_KEY_KP_4;
    windowData->keycodes[SDLK_KP_5] = TIGOR_KEY_KP_5;
    windowData->keycodes[SDLK_KP_6] = TIGOR_KEY_KP_6;
    windowData->keycodes[SDLK_KP_7] = TIGOR_KEY_KP_7;
    windowData->keycodes[SDLK_KP_8] = TIGOR_KEY_KP_8;
    windowData->keycodes[SDLK_KP_9] = TIGOR_KEY_KP_9;
    windowData->keycodes[SDLK_KP_PLUS] = TIGOR_KEY_KP_ADD;
    windowData->keycodes[SDLK_KP_DECIMAL] = TIGOR_KEY_KP_DECIMAL;
    windowData->keycodes[SDLK_KP_DIVIDE] = TIGOR_KEY_KP_DIVIDE;
    windowData->keycodes[SDLK_KP_ENTER] = TIGOR_KEY_KP_ENTER;
    windowData->keycodes[SDLK_KP_EQUALS] = TIGOR_KEY_KP_EQUAL;
    windowData->keycodes[SDLK_KP_MULTIPLY] = TIGOR_KEY_KP_MULTIPLY;
    windowData->keycodes[SDLK_KP_MINUS] = TIGOR_KEY_KP_SUBTRACT;

    for (scancode = 0;  scancode < 512;  scancode++)
    {
        if (windowData->keycodes[scancode] > 0)
            windowData->scancodes[windowData->keycodes[scancode]] = scancode;
    }
}

void _wManagerUpdateKeyNamesWeb(void)
{
    int key;
    char state[256] = {0};

    memset(((wManagerWeb* )_wMWindow.WindowData)->keynames, 0, sizeof(((wManagerWeb* )_wMWindow.WindowData)->keynames));

}


uint32_t _wManagerInitWeb(void)
{
    _wManagerUpdateKeyNamesWeb();
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
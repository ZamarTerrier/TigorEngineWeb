#include "window_manager.h"

#include "e_window.h"

wManagerWindow _wMWindow;

extern TEngine engine;

// Internal key state used for sticky keys
#define _TIGOR_STICK 3

extern void createKeyTables(void* wData);

void wManagerDefaultWindowHints(wManagerWindow *window)
{
    // The default is a focused, visible, resizable window with decorations
    memset(&window->hints.window, 0, sizeof(window->hints.window));
    window->hints.window.resizable    = true;
    window->hints.window.visible      = true;
    window->hints.window.decorated    = true;
    window->hints.window.focused      = true;
    window->hints.window.autoIconify  = true;
    window->hints.window.centerCursor = true;
    window->hints.window.focusOnShow  = true;
    window->hints.window.xpos         = TIGOR_ANY_POSITION;
    window->hints.window.ypos         = TIGOR_ANY_POSITION;

    // The default is 24 bits of color, 24 bits of depth and 8 bits of stencil,
    // double buffered
    memset(&window->hints.framebuffer, 0, sizeof(window->hints.framebuffer));
    window->hints.framebuffer.redBits      = 8;
    window->hints.framebuffer.greenBits    = 8;
    window->hints.framebuffer.blueBits     = 8;
    window->hints.framebuffer.alphaBits    = 8;
    window->hints.framebuffer.depthBits    = 24;
    window->hints.framebuffer.stencilBits  = 8;
    window->hints.framebuffer.doublebuffer = true;

    // The default is to select the highest available refresh rate
    window->hints.refreshRate = TIGOR_DONT_CARE;

    // The default is to use full Retina resolution framebuffers
    window->hints.window.ns.retina = true;

    window->cursorMode = TIGOR_CURSOR_NORMAL;
}

void wManagerTerminate()
{
    TWindow *window = (TWindow *)engine.window;

    wManagerWindow *wManager = window->e_window;

    free(wManager->WindowData);
    free(_wMWindow.WindowData);

    free(window->e_window);
}


int wManagerInit(){    
    TWindow *window = (TWindow *)engine.window;
    
    window->e_window = calloc(1, sizeof(wManagerWindow));

    wManagerWindow *wManager = window->e_window;

    wManager->WindowData = calloc(1, sizeof(wManagerWeb));
    _wMWindow.WindowData = calloc(1, sizeof(wManagerWeb));
    
    extern int32_t _wManagerConnectWeb(_wManagerPlatform* platform);
    
    _wManagerConnectWeb(&_wMWindow.platform);
    _wManagerConnectWeb(&wManager->platform);

    if (!_wMWindow.platform.init())
    {
        wManagerTerminate();
        return false;
    }
    
    wManagerDefaultWindowHints(&_wMWindow);
    wManagerDefaultWindowHints(window->e_window);
    
    createKeyTables(_wMWindow.WindowData);
    createKeyTables(wManager->WindowData);

}

int wManagerGetMouseButton(wManagerWindow *window, int button)
{
    if (button < TIGOR_MOUSE_BUTTON_1 || (button > TIGOR_MOUSE_BUTTON_LAST))
    {
        printf("Invalid mouse button %i\n", button);
        return TIGOR_RELEASE;
    }

    if (window->mouseButtons[button] == _TIGOR_STICK)
    {
        // Sticky mode: release mouse button now
        window->mouseButtons[button] = TIGOR_RELEASE;
        return TIGOR_PRESS;
    }

    return (int) window->mouseButtons[button];
}

int wManagerGetKey(wManagerWindow *window, int key)
{

    if (key < TIGOR_KEY_SPACE || (key > TIGOR_KEY_LAST))
    {
        printf("Invalid key %i\n", key);
        return TIGOR_RELEASE;
    }

    if (window->keys[key] == _TIGOR_STICK)
    {
        // Sticky mode: release key now
        window->keys[key] = TIGOR_RELEASE;
        return TIGOR_PRESS;
    }

    return (int) window->keys[key];
}

void wManagerGetCursorPos(wManagerWindow *window, double *xpos, double *ypos)
{
    window->platform.getCursorPos(window, xpos, ypos);
}

void wManagerSetCursorPos(wManagerWindow *window, double xpos, double ypos)
{
    window->platform.setCursorPos(window, xpos, ypos);
}
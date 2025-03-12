#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "engine_includes.h"


#include "manager_includes.h"

#include "web_defines.h"

#define WINPARAM void*

typedef void (*wManagerMonitorFun)(_wManagerMonitor* monitor, int event);

typedef void (*wManagerWindowCloseFun)(wManagerWindow* window);
typedef void (*wManagerCursorEnter)(wManagerWindow* , int32_t);
typedef void (*wManagerCharacterFunc)(wManagerWindow*, uint32_t);
typedef void (*wManagerWindowRefreshFun)(wManagerWindow* window);
typedef void (*wManagerCursorPos)(wManagerWindow* , int32_t, int32_t);
typedef void (*wManagerCharModsFunc)(wManagerWindow*, uint32_t, uint32_t);
typedef void (*wManagerWindowFocusFun)(wManagerWindow* window, int focused);
typedef void (*wManagerWindowiConifyFun)(wManagerWindow* window, int iconified);
typedef void (*wManagerWindowMaximizeFun)(wManagerWindow* window, int maximized);
typedef void (*wManagerMouseButtonFun)(wManagerWindow* , int32_t, int32_t, int32_t);
typedef void (*wManagerCursorPosFun)(wManagerWindow* window, double xpos, double ypos);
typedef void (*wManageWindowPosFun)(wManagerWindow* window, int32_t xpos, int32_t ypos);
typedef void (*wManagerKeyFunc)(wManagerWindow*, int, int, int, int);
typedef void (*wManagerScrollfFn)(wManagerWindow* window, double xoffset, double yoffset);
typedef void (*wManagerDropFun)(wManagerWindow* window, int path_count, const char* paths[]);
typedef void (*wManagerWindowSizeFunc)(wManagerWindow *window, int32_t width, int32_t height);
typedef void (*wManagerWindowContentScaleFun)(wManagerWindow* window, float xscale, float yscale);
typedef void (*wManagerFrameBufferSizeFun)(wManagerWindow* window, int32_t width, int32_t height);

typedef struct wManagerWindow{

    _wManagerPlatform platform;

    struct wManagerWindow *next;

    struct wManagerWindow *windowListHead;

    wManagerVidmode         videoMode;
    _wManagerMonitor*       monitor;

    _wManagerMonitor**      monitors;
    int                 monitorCount;

    int32_t frameAction;
    int32_t cursorTracked;
    int32_t iconified;
    int32_t maximized;
    int32_t transparent;
    int32_t scaleToMonitor;
    int32_t keymenu;
    int32_t rawMouseMotion;
    int32_t showDefault;
    int32_t decorated;
    int32_t floating;

    int32_t shouldClose;

    int32_t stickyKeys;
    int32_t stickyMouseButtons;
    int32_t lockKeyMods;
    int     cursorMode;
    char    mouseButtons[TIGOR_MOUSE_BUTTON_LAST + 1];
    char    fingers[10];
    char    keys[TIGOR_KEY_LAST + 1];

    double virtualCursorPosX, virtualCursorPosY;

    // Cached size used to filter out duplicate events
    int width, height;

    int minwidth, minheight;
    int maxwidth, maxheight;
    int numer, denom;

    void *WindowData;

    uint32_t isRemoteSession;

    uint64_t time_freq;
    uint64_t time_clock;
    uint64_t time_offset;

    _wManagercontext context;

    struct {
            _wManagerinitconfig init;
            _wManagerfbconfig   framebuffer;
            _wManagerwndconfig  window;
            int             refreshRate;
        } hints;

    struct {
            wManageWindowPosFun pos;
            wManagerWindowSizeFunc size;
            wManagerWindowCloseFun close;
            wManagerWindowRefreshFun refresh;
            wManagerWindowFocusFun focus;
            wManagerWindowiConifyFun iconify;
            wManagerWindowMaximizeFun maximize;
            wManagerFrameBufferSizeFun fbsize;
            wManagerWindowContentScaleFun scale;
            wManagerMouseButtonFun mouseButton;
            wManagerCursorPos cursorPos;
            wManagerCursorEnter cursorEnter;
            wManagerScrollfFn scroll;
            wManagerKeyFunc key;
            wManagerCharacterFunc character;
            wManagerCharModsFunc charmods;
            wManagerDropFun drop;

            wManagerMonitorFun  monitor;
        } callbacks;

} wManagerWindow;

int wManagerInit();

int wManagerGetKey(wManagerWindow *window, int key);
void wManagerGetCursorPos(wManagerWindow *window, double *xpos, double *ypos);
void wManagerSetCursorPos(wManagerWindow *window, double xpos, double ypos);
int wManagerGetMouseButton(wManagerWindow *window, int button);

void wManagerSetKeyCallback(wManagerWindow *window, wManagerKeyFunc EngineKeyCallback);
void wManagerSetMouseButtonCallback(wManagerWindow *window, wManagerMouseButtonFun func);
void wManagerSetCursorPosCallback(wManagerWindow *window, wManagerCursorPosFun callback);

#endif // WINDOW_MANAGER_H
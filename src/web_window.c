#include "engine_includes.h"
#include "window_manager.h"

extern TEngine engine;

extern wManagerWindow _wMWindow;

#define KF_EXTENDED 0x0100

void normWindowToDeviceCoords (wManagerWindow *window, float normWinX, float normWinY, float *deviceX, float *deviceY)
{
    /*deviceX = (normWinX - 0.5f) * 2.0f;
    deviceY = (1.0f - normWinY - 0.5f) * 2.0f;*/
}

void deviceToWorldCoords (wManagerWindow *window, float deviceX, float deviceY, float *worldX, float *worldY)
{
    /*worldX = deviceX / mZoom - mPan.x;
    worldY = deviceY / mAspect / mZoom - mPan.y;*/
}


void normWindowToWorldCoords(wManagerWindow *window, float normWinX, float normWinY, float *worldX, float *worldY)
{
    float deviceX, deviceY;
    normWindowToDeviceCoords(window, normWinX, normWinY, &deviceX, &deviceY);
    deviceToWorldCoords(window, deviceX, deviceY, worldX, worldY);
}

void windowToDeviceCoords (wManagerWindow *window, int winX, int winY, float *deviceX, float *deviceY)
{
    normWindowToDeviceCoords(window, winX / (float)engine.width,  winY / (float)engine.height, deviceX, deviceY);
}

void panEventMouse(wManagerWindow *window, int x, int y)
{ 
    wManagerWeb *wManager = (wManagerWeb *)window->WindowData;

    wManager->lastCursorPosX = x;
    wManager->lastCursorPosY = y;

    /* int deltaX = engine.width / 2 + (x - wManager->mMouseButtonDownX),
         deltaY = engine.height / 2 + (y - wManager->mMouseButtonDownY);

    float deviceX, deviceY;
    windowToDeviceCoords(deltaX,  deltaY, deviceX, deviceY);

    Vec2 pan = { mCamera.basePan().x + deviceX / mCamera.zoom(), 
                 mCamera.basePan().y + deviceY / mCamera.zoom() / mCamera.aspect() };
    mCamera.setPan(pan);*/
}
void _wManagerSetCursorPosWeb(wManagerWindow *window, double xpos, double ypos)
{

}

void _wManagerGetCursorPosWeb(wManagerWindow *window, double* xpos, double* ypos)
{
    wManagerWeb *wManager = (wManagerWeb *)window->WindowData;

    if (xpos)
        *xpos = wManager->lastCursorPosX;
    if (ypos)
        *ypos = wManager->lastCursorPosY;
}

const char* _wManagerGetScancodeNameWeb(int scancode)
{
    if (scancode < 0 || scancode > (KF_EXTENDED | 0xff))
    {
        printf("Invalid scancode %i\n", scancode);
        return NULL;
    }

    const int key = ((wManagerWeb* )_wMWindow.WindowData)->keycodes[scancode];
    if (key == TIGOR_KEY_UNKNOWN)
        return NULL;

    return ((wManagerWeb* )_wMWindow.WindowData)->keynames[key];
}

int _wManagerGetKeyScancodeWeb(int key)
{
    return ((wManagerWeb* )_wMWindow.WindowData)->scancodes[key];
}

void _wManagerPoolEventWeb(wManagerWindow *window, SDL_Event event){
    
    wManagerWeb *wManager = (wManagerWeb *)window->WindowData;

    switch (event.type)
    {
        case SDL_KEYDOWN:
            window->keys[wManager->keycodes[event.key.keysym.sym]] = TIGOR_PRESS;

            break;
        case SDL_KEYUP:
            window->keys[wManager->keycodes[event.key.keysym.sym]] = TIGOR_RELEASE;

            break;
        
        case SDL_MOUSEWHEEL: 
        {
            // SDL_MOUSEWHEEL regression? 
            // m->y no longer reliable (often y is 0 when mouse wheel is spun up or down), use m->preciseY instead
            SDL_MouseWheelEvent *m = (SDL_MouseWheelEvent*)&event;
        	#ifdef EVENTS_DEBUG
            	printf ("SDL_MOUSEWHEEL= x,y=%d,%d preciseX,preciseY=%f,%f\n", m->x, m->y, m->preciseX, m->preciseY);
        	#endif
        	bool mouseWheelDown = (m->preciseY < 0.0);
        	//zoomEventMouse(mouseWheelDown, mMousePositionX, mMousePositionY);
        	break;
        }
        case SDL_MOUSEMOTION: 
        {
            SDL_MouseMotionEvent *m = (SDL_MouseMotionEvent*)&event;
            if (wManager->mMouseButtonDown && !wManager->mFingerDown && !wManager->mPinch)
                panEventMouse(window, m->x, m->y);
            break;
        }
        case SDL_MOUSEBUTTONDOWN: 
        {
            SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent*)&event;
            if (m->button == SDL_BUTTON_LEFT && !wManager->mFingerDown && !wManager->mPinch)
            {
                wManager->mMouseButtonDown = true;
                wManager->mMouseButtonDownX = m->x;
                wManager->mMouseButtonDownY = m->y;
                //mCamera.setBasePan();
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: 
        {
            SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent*)&event;
            if (m->button == SDL_BUTTON_LEFT)
                wManager->mMouseButtonDown = false;
            break;
        }
        case SDL_FINGERDOWN:
            if (!wManager->mPinch)
            {
                // Finger already down means multiple fingers, which is handled by multigesture event
                if (wManager->mFingerDown)
                    wManager->mFingerDown = false;
                else
                {
                    SDL_TouchFingerEvent *m = (SDL_TouchFingerEvent*)&event;

                    wManager->mFingerDown = true;
                    wManager->mFingerDownX = m->x;
                    wManager->mFingerDownY = m->y;
                    wManager->mFingerDownId = m->fingerId;
                    //mCamera.setBasePan();
                }
            }
            break;            
        case SDL_FINGERUP:
            wManager->mFingerDown = false;
            wManager->mPinch = false;
            break;

    }

}

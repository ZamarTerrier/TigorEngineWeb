#include "engine_includes.h"
#include "window_manager.h"

#include "input_manager.h"

extern TEngine engine;

extern wManagerWindow _wMWindow;

#define KF_EXTENDED 0x0100

// Retrieves and translates modifier keys
//
int getKeyMods(void)
{
    int mods = 0;

    SDL_Keymod modstate = SDL_GetModState();

    if (modstate & KMOD_SHIFT)
        mods |= TIGOR_MOD_SHIFT;
    if (modstate & KMOD_CTRL)
        mods |= TIGOR_MOD_CONTROL;
    if (modstate & KMOD_ALT)
        mods |= TIGOR_MOD_ALT;
    if (modstate & KMOD_GUI)
        mods |= TIGOR_MOD_SUPER;
    if (modstate & KMOD_CAPS)
        mods |= TIGOR_MOD_CAPS_LOCK;
    if (modstate & KMOD_NUM)
        mods |= TIGOR_MOD_NUM_LOCK;

    return mods;
}

int _wManagerSetCursorPosWeb(wManagerWindow *window, double xpos, double ypos)
{
    return 0;
}

void _wManagerGetCursorPosWeb(wManagerWindow *window, double* xpos, double* ypos)
{
    wManagerWeb *wManager = (wManagerWeb *)window->WindowData;

    if (xpos)
        *xpos = wManager->lastCursorPosX;
    if (ypos)
        *ypos = wManager->lastCursorPosY;
}

int _wManagerGetMousePressState(wManagerWindow *window){
    wManagerWeb *wManager = (wManagerWeb *)window->WindowData;

    return wManager->mMouseButtonDown;
}

WebFinger *_wManagerGetFingerPressState(wManagerWindow *window){
    wManagerWeb *wManager = (wManagerWeb *)window->WindowData;

    return wManager->fingers;
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

    int button, action;
    int dx, dy;

    SDL_TouchFingerEvent *m;

    
    wManager->someFingerDown = false;

    for(int i=0;i < 10;i++){
        if(wManager->fingers[i].mFingerDown == TIGOR_PRESS)
            wManager->someFingerDown = true;
    }

    switch (event.type)
    {
        case SDL_KEYDOWN:
            action = TIGOR_PRESS;
            _wManagerInputKey(window, wManager->keycodes[event.key.keysym.scancode], event.key.keysym.scancode, action, getKeyMods());
            break;
        case SDL_KEYUP:
            action = TIGOR_RELEASE;
            _wManagerInputKey(window, wManager->keycodes[event.key.keysym.scancode], event.key.keysym.scancode, action, getKeyMods());
            break;
        
        case SDL_MOUSEWHEEL: 
        {
            // SDL_MOUSEWHEEL regression? 
            // m->y no longer reliable (often y is 0 when mouse wheel is spun up or down), use m->preciseY instead
            SDL_MouseWheelEvent *m = (SDL_MouseWheelEvent*)&event;
        	#ifdef EVENTS_DEBUG
            	printf ("SDL_MOUSEWHEEL= x,y=%d,%d preciseX,preciseY=%f,%f\n", m->x, m->y, m->preciseX, m->preciseY);
        	#endif
        	bool mouseWheelDown = (m->y < 0.0);
        	//zoomEventMouse(mouseWheelDown, mMousePositionX, mMousePositionY);
            _wManagerInputScroll(window, 0.0, m->y);

        }
            break;
        case SDL_MOUSEMOTION: 
        {
            SDL_MouseMotionEvent *m = (SDL_MouseMotionEvent*)&event;
            if (!wManager->someFingerDown)
            {
                  
                dx = m->x;
                dy = m->y;

                _wManagerInputCursorPos(window, ((wManagerWeb* )window->WindowData)->virtualCursorPosX + dx,
                                            ((wManagerWeb* )window->WindowData)->virtualCursorPosY + dy);
                                                 
                ((wManagerWeb* )window->WindowData)->lastCursorPosX = dx;
                ((wManagerWeb* )window->WindowData)->lastCursorPosY = dy;
                
            }

        }
            break;
        case SDL_MOUSEBUTTONDOWN: 
            {
                SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent*)&event;
                if(!wManager->someFingerDown)
                {
                    switch(m->button){
                        case SDL_BUTTON_LEFT:
                            button = TIGOR_MOUSE_BUTTON_LEFT;
                            wManager->mMouseButtonDown = TIGOR_PRESS;             
                            wManager->mMouseButtonDownX = m->x;
                            wManager->mMouseButtonDownY = m->y;
                            break;
                        case SDL_BUTTON_MIDDLE:
                            button = TIGOR_MOUSE_BUTTON_MIDDLE;
                            break;
                        case SDL_BUTTON_RIGHT:
                            button = TIGOR_MOUSE_BUTTON_RIGHT;
                            break;
                    }

                    action = TIGOR_PRESS;
                    
                    _wManagerInputMouseClick(window, button, action, getKeyMods());
                }
            }
            break;

        case SDL_MOUSEBUTTONUP: 
        {
            SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent*)&event;
            if(!wManager->someFingerDown)
            {
            
                switch(m->button){
                    case SDL_BUTTON_LEFT:
                        button = TIGOR_MOUSE_BUTTON_LEFT;
                        wManager->mMouseButtonDown = TIGOR_RELEASE; 
                        break;
                    case SDL_BUTTON_MIDDLE:
                        button = TIGOR_MOUSE_BUTTON_MIDDLE;
                        break;
                    case SDL_BUTTON_RIGHT:
                        button = TIGOR_MOUSE_BUTTON_RIGHT;
                        break;
                }
                
                action = TIGOR_RELEASE;
                    
                _wManagerInputMouseClick(window, button, action, getKeyMods());
            }
        }
            break;
        case SDL_FINGERMOTION:
            m = (SDL_TouchFingerEvent *)&event;

            wManager->fingers[event.tfinger.fingerId%10].mFingerDownX = m->x;
            wManager->fingers[event.tfinger.fingerId%10].mFingerDownY = m->y;

            break;
        case SDL_FINGERDOWN:
            m = (SDL_TouchFingerEvent *)&event;
            wManager->someFingerDown = true;
            wManager->fingers[event.tfinger.fingerId%10].mFingerDown = TIGOR_PRESS;
            wManager->fingers[event.tfinger.fingerId%10].mFingerDownX = m->x;
            wManager->fingers[event.tfinger.fingerId%10].mFingerDownY = m->y;
            
            _wManagerInputFinger(window, event.tfinger.fingerId, TIGOR_PRESS);
            //mCamera.setBasePan();
            break;            
        case SDL_FINGERUP:
            m = (SDL_TouchFingerEvent *)&event;
            wManager->fingers[event.tfinger.fingerId%10].mFingerDown = TIGOR_RELEASE;
            _wManagerInputFinger(window, event.tfinger.fingerId, TIGOR_RELEASE);
            break;

    }

}

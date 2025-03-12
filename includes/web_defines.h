#ifndef WEB_DEFINES_H
#define WEB_DEFINES_H

#include "manager_includes.h"

#include "key_defines.h"

typedef struct{
        int                 acquiredMonitorCount;

        char*               clipboardString;
        short int           keycodes[512];
        short int           scancodes[TIGOR_KEY_LAST + 1];
        char                keynames[TIGOR_KEY_LAST + 1][5];

        double restoreCursorPosX, restoreCursorPosY;
        double virtualCursorPosX, virtualCursorPosY;
        double lastCursorPosX , lastCursorPosY;

        int mMouseButtonDown;
        double mMouseButtonDownX, mMouseButtonDownY;

        WebFinger fingers[10];
        
        bool someFingerDown;
                
} wManagerWeb;

#endif //WEB_DEFINES_H

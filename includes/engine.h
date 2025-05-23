/// @file Core/engine.h
#ifndef GAMENGINE_H
#define GAMENGINE_H

#include "engine_includes.h"

#include "gameObject.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*SomeUpdateFunc)(float deltaTime);
typedef void (*SomeKeyCallbackFunc)(void *wManager, int key, int scancode, int action, int mods);
typedef void (*SomeMouseKeyCallbackFunc)(void *wManager, int button, int action, int mods);
typedef void (*SomeFingerKeyCallbackFunc)(void *wManager, int fingerID, int action);

void TEngineInitSystem(int width, int height, const char* name);
void TEngineDraw(GameObject *go);
void TEngineSetUpdater(SomeUpdateFunc update);
void TEngineSetKeyCallback(SomeKeyCallbackFunc callback);
void TEngineSetMouseKeyCallback(SomeMouseKeyCallbackFunc callback);
void TEngineSetFingerCallback(SomeFingerKeyCallbackFunc callback);
void TEngineRender();

void TEngineSetFont(char *font_path);

int TEngineGetMousePress(int Key);
void TEngineGetCursorPos(double *xpos, double *ypos);
void TEngineSetCursorPos(float xpos, float ypos);
int TEngineGetKeyPress(int Key);
const char *TEngineGetClipBoardString();
void TEngineSetClipBoardString(const char *string);
void TEngineGetWindowSize(int *width, int *height);
int TEngineGetMousePressState();
void *TEngineGetFingerPressState();

#ifdef __cplusplus
}
#endif

#endif

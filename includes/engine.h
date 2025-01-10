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

void TEngineInitSystem(int width, int height, const char* name);
void TEngineDraw(GameObject *go);
void TEngineSetUpdater(SomeUpdateFunc *update);
void TEngineRender();

int TEngineGetMousePress(int Key);
void TEngineGetCursorPos(double *xpos, double *ypos);
void TEngineSetCursorPos(float xpos, float ypos);
int TEngineGetKeyPress(int Key);

#ifdef __cplusplus
}
#endif

#endif

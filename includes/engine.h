/// @file Core/engine.h
#ifndef GAMENGINE_H
#define GAMENGINE_H

#include "engine_includes.h"

#include "gameObject.h"

#ifdef __cplusplus
extern "C"
{
#endif

void TEngineInitSystem(int width, int height, const char* name);
void TEngineDraw(GameObject *go);
void TEngineRender();

#ifdef __cplusplus
}
#endif

#endif

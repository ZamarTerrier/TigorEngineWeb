#ifndef E_VERTEX_H
#define E_VERTEX_H

#include "e_math_variables.h"

typedef struct {
    vec2 position;
    vec3 color;
    vec2 texCoord;
} Vertex2D;

typedef struct {
    vec2 position;
    vec4 color;
    vec2 texCoord;
} Vertex2DGUI;

#endif // E_VERTEX_H
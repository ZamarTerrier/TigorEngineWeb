#ifndef E_TOOLS_H
#define E_TOOLS_H

#include "engine_includes.h"

#include "graphicsItems.h"

#ifdef __cplusplus
extern "C"
{
#endif

float dot_grad(int hash, float xf);
float dot_grad2(int hash, float xf, float yf);

vec3 perp(const vec3 v);

float fade(float t);

float PerlinNoise1D(float x);
float PerlinNoise2D(float x, float y);
float PerlinOctave1D(uint32_t octaves, float x, float frequency, float amplitude);
float PerlinOctave2D(uint32_t octaves, float x, float y, float frequency, float amplitude);

float SimplexNoise1D(float x);
float SimplexNoise2D(float x, float y);
float SimplexOctave1D(uint32_t octaves, float x, float frequency, float amplitude);
float SimplexOctave2D(uint32_t octaves, float x, float y, float frequency, float amplitude);

float sinWithRange(float value, float minV, float range);
float cosWithRange(float value, float minV, float range);

int IcoSphereGenerator(vertexParam *vParam, indexParam *iParam,float radius);
int Cubesphere(vertexParam *vParam, indexParam *iParam, float radius,int vertexCountPerRow);
int SphereGenerator3D(vertexParam *vParam, indexParam *iParam,float radius, int stackCount, int sectorCount);
void ConeGenerator(vertexParam *vParam, indexParam *iParam, const float height, int stackCount, int sectorCount);
void InitPlane3D(vertexParam *vParam, indexParam *iParam, int stackCount, int sectorCount);

char *ToolsMakeString(char *s1, char *s2);
void ToolsAddStrings(char *out, int buff_size, char *s1, char *s2);
bool ToolsCmpStrings(char *in, char *s1);
int ToolsStr32BitLength(uint32_t *text);
int ToolsTextStrFromUtf8(uint32_t* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_text_remaining);
int ToolsStringToUInt32(uint32_t *dest, const char *src);
int ToolsUInt32ToString(char *dest, const uint32_t *src);

void* ToolsLoadImageFromFile(size_t* len, char *filepath);

#ifdef __cplusplus
}
#endif

#endif // E_TOOLS_H

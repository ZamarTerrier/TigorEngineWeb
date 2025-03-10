#include "e_tools.h"
#include "e_math.h"

#include <math.h>
#include <string.h>

#include "e_memory.h"

#include "e_vertex.h"

#define TIGOR_UNICODE_CODEPOINT_MAX 0xFFFF
#define TIGOR_UNICODE_CODEPOINT_INVALID 0xFFFD

extern TEngine engine;

const FontIndexes fontIndexes[] ={
    { 1025, 0xD081 },//Ё
    { 1040, 0xD090 },//А
    { 1041, 0xD091 },//Б
    { 1042, 0xD092 },//В
    { 1043, 0xD093 },//Г
    { 1044, 0xD094 },//Д
    { 1045, 0xD095 },//Е
    { 1046, 0xD096 },//Ж
    { 1047, 0xD097 },//З
    { 1048, 0xD098 },//И
    { 1049, 0xD099 },//Й
    { 1050, 0xD09A },//К
    { 1051, 0xD09B },//Л
    { 1052, 0xD09C },//М
    { 1053, 0xD09D },//Н
    { 1054, 0xD09E },//О
    { 1055, 0xD09F },//П
    { 1056, 0xD0A0 },//Р
    { 1057, 0xD0A1 },//С
    { 1058, 0xD0A2 },//Т
    { 1059, 0xD0A3 },//У
    { 1060, 0xD0A4 },//Ф
    { 1061, 0xD0A5 },//Х
    { 1062, 0xD0A6 },//Ц
    { 1063, 0xD0A7 },//Ч
    { 1064, 0xD0A8 },//Ш
    { 1065, 0xD0A9 },//Щ
    { 1066, 0xD0AA },//Ъ
    { 1067, 0xD0AB },//Ы
    { 1068, 0xD0AC },//Ь
    { 1069, 0xD0AD },//Э
    { 1070, 0xD0AE },//Ю
    { 1071, 0xD0AF },//Я
    { 1072, 0xD0B0 },//а
    { 1073, 0xD0B1 },//б
    { 1074, 0xD0B2 },//в
    { 1075, 0xD0B3 },//г
    { 1076, 0xD0B4 },//д
    { 1077, 0xD0B5 },//е
    { 1078, 0xD0B6 },//ж
    { 1079, 0xD0B7 },//з
    { 1080, 0xD0B8 },//и
    { 1081, 0xD0B9 },//й
    { 1082, 0xD0BA },//к
    { 1083, 0xD0BB },//л
    { 1084, 0xD0BC },//м
    { 1085, 0xD0BD },//н
    { 1086, 0xD0BE },//о
    { 1087, 0xD0BF },//п
    { 1088, 0xD180 },//р
    { 1089, 0xD181 },//с
    { 1090, 0xD182 },//т
    { 1091, 0xD183 },//у
    { 1092, 0xD184 },//ф
    { 1093, 0xD185 },//х
    { 1094, 0xD186 },//ц
    { 1095, 0xD187 },//ч
    { 1096, 0xD188 },//ш
    { 1097, 0xD189 },//щ
    { 1098, 0xD18A },//ъ
    { 1099, 0xD18B },//ы
    { 1100, 0xD18C },//ь
    { 1101, 0xD18D },//э
    { 1102, 0xD18E },//ю
    { 1103, 0xD18F },//я
    { 1105, 0xD191 },//ё
};

int p[512] = {
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
    8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203,
    117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
    165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220,
    105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
    187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186,
    3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
    227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70,
    221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
    185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241,
    81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176,
    115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195,
    78, 66, 215, 61, 156, 180,

    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
    8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203,
    117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
    165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220,
    105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
    187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186,
    3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
    227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70,
    221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
    185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241,
    81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176,
    115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195,
    78, 66, 215, 61, 156, 180,
};

const double epsilon = 2.718281828182818281828;

float dot_grad(int hash, float xf){
        // In 1D case, the gradient may be either 1 or -1
        // The distance vector is the input offset (relative to the smallest bound)
        return (hash & 0x1) ? xf : -xf;
}

float dot_grad2(int hash, float xf, float yf){
    // In 2D case, the gradient may be any of 8 direction vectors pointing to the
    // edges of a unit-square. The distance vector is the input offset (relative to
    // the smallest bound)
    switch (hash & 0x7) {
        case 0x0: return  xf + yf;
        case 0x1: return  xf;
        case 0x2: return  xf - yf;
        case 0x3: return -yf;
        case 0x4: return -xf - yf;
        case 0x5: return -xf;
        case 0x6: return -xf + yf;
        case 0x7: return  yf;
        default:  return  0.0f;
    }
}

float gradv1(int hash, float x)
{
    const int h = hash & 0x0F;
    float grad = 1.0f + (h & 7);
    if((h & 8) != 0) grad = -grad;

    return grad * x;
}

float gradv2(int hash, float x, float y)
{
    const int h = hash & 0x3F;
    const float u = h < 4 ? x : y;
    const float v = h < 4 ? y : x;

    return((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

float gradv3(int hash, float x, float y, float z)
{
    const int h = hash & 15;
    const float u = h < 8 ? x : y;
    const float v = h < 4 ? y : h == 12 || h == 14 ? x : z;

    return((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

vec3 perp(const vec3 v) {
    float min = fabs(v.x);
    vec3 cardinalAxis = {1, 0, 0};

    if (fabs(v.y) < min) {
        min = fabsf(v.y);
        cardinalAxis = (vec3){0, 1, 0};
    }

    if (fabs(v.z) < min) {
        cardinalAxis = (vec3){0, 0, 1};
    }

    return v3_cross(v, cardinalAxis);
}

float lerp_noise(float a, float b, float t) {
    return a + t * (b - a);
}

float fade(float t){
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise1D( float x){
    // Left coordinate of the unit-line that contains the input
    int xi0 = floor(x);

    // Input location in the unit-line
    float xf0 = x - xi0;
    float xf1 = xf0 - 1.0;

    // Wrap to range 0-255
    int const xi = xi0 & 0xFF;

    // Apply the fade function to the location
    float const u = fade(xf0);

    // Generate hash values for each point of the unit-line
    int const h0 = p[xi + 0];
    int const h1 = p[xi + 1];

    // Linearly interpolate between dot products of each gradient with its distance to the input location
    return lerp_noise(dot_grad(h0, xf0), dot_grad(h1, xf1), u);
}

float PerlinNoise2D(float x, float y)
{
    // Top-left coordinates of the unit-square
    int xi0 = (int)(floor(x)) & 0xFF;
    int yi0 = (int)(floor(y)) & 0xFF;

    // Input location in the unit-square
    float xf0 = x - xi0;
    float yf0 = y - yi0;
    float xf1 = xf0 - 1.0;
    float yf1 = yf0 - 1.0;

    // Wrap to range 0-255
    int const xi = xi0 & 0xFF;
    int const yi = yi0 & 0xFF;

    // Apply the fade function to the location
    float const u = fade(xf0);
    float const v = fade(yf0);

    // Generate hash values for each point of the unit-square
    int const h00 = p[p[xi + 0] + yi + 0];
    int const h01 = p[p[xi + 0] + yi + 1];
    int const h10 = p[p[xi + 1] + yi + 0];
    int const h11 = p[p[xi + 1] + yi + 1];

    // Linearly interpolate between dot products of each gradient with its distance to the input location
    float const x1 = lerp_noise(dot_grad2(h00, xf0, yf0), dot_grad2(h10, xf1, yf0), u);
    float const x2 = lerp_noise(dot_grad2(h01, xf0, yf1), dot_grad2(h11, xf1, yf1), u);
    return lerp_noise(x1, x2, v);
}

float SimplexNoise1D(float x)
{
    float n0, n1;

    int i0 = floor(x);
    int i1 = i0 + 1;

    float x0 = x - i0;
    float x1 = x0 - 1.0f;

    float t0 = 1.0f - x0 * x0;

    t0 *= t0;

    uint8_t h = i0;
    n0 = t0 * t0 * gradv1(p[h], x1);

    float t1 = 1.0 - x1 * x1;

    t1 *= t1;
    h = i1;
    n1 = t1 * t1 * gradv1(p[h], x1);

    return 0.395f * (n0 + n1);
}

float SimplexNoise2D(float x, float y)
{
    float n0, n1, n2;

    const float F2 = 0.366025403f;
    const float G2 = 0.211324865f;

    const float s = (x + y) * F2;
    const float xs = x + s;
    const float ys = y + s;
    const int i = floor(xs);
    const int j = floor(ys);

    const float t = (float)(i + j) * G2;
    const float X0 = i - t;
    const float Y0 = j - t;
    const float x0 = x - X0;
    const float y0 = y - Y0;

    int i1, j1;
    if(x0 > y0){
        i1 = 1;
        j1 = 0;
    }else{
        i1 = 0;
        j1 = 1;
    }

    const float x1 = x0 - i1 + G2;
    const float y1 = y0 - j1 + G2;
    const float x2 = x0 - 1.0f + 2.0f * G2;
    const float y2 = y0 - 1.0f + 2.0f * G2;

    const int gi0 = p[(uint8_t)i + p[(uint8_t)j]];
    const int gi1 = p[(uint8_t)i + (uint8_t)i1 + p[(uint8_t)j + (uint8_t)j1]];
    const int gi2 = p[(uint8_t)i + 1 + p[(uint8_t)j + 1]];

    float t0 = 0.5f - x0 * x0 - y0 * y0;
    if(t0 < 0.0f){
        n0 = 0.0f;
    }else{
        t0 *= t0;
        n0 = t0 * t0 * gradv2(gi0, x0, y0);
    }

    float t1 = 0.5f - x1 * x1 - y1 * y1;
    if(t1 < 0.0f){
        n1 = 0.0f;
    }else{
        t1 *= t1;
        n1 = t1 * t1 * gradv2(gi1, x1, y1);
    }

    float t2 = 0.5f - x2 * x2 - y2 * y2;
    if(t2 < 0.0f){
        n2 = 0.0f;
    }else{
        t2 *= t2;
        n2 = t2 * t2 * gradv2(gi2, x2, y2);
    }

    return 45.23065f * (n0 + n1 + n2);
}

float PerlinOctave1D(uint32_t octaves, float x, float frequency, float amplitude){
    float output = 0;
    float denom = 0;

    for(int i=0;i < octaves; i++)
    {
        output += (amplitude * PerlinNoise1D(x * frequency));
        denom += amplitude;

        frequency *= 2.0;
        amplitude *=0.5;
    }

    return output / denom;
}

float PerlinOctave2D(uint32_t octaves, float x, float y, float frequency, float amplitude){
    float output = 0;
    float denom = 0;

    for(int i=0;i < octaves; i++)
    {
        output += (amplitude * PerlinNoise2D(x * frequency, y * frequency));
        denom += amplitude;

        frequency *= 2.0;
        amplitude *=0.5;
    }

    return output / denom;
}

float SimplexOctave1D(uint32_t octaves, float x, float frequency, float amplitude){
    float output = 0;
    float denom = 0;

    for(int i=0;i < octaves; i++)
    {
        output += (amplitude * SimplexNoise1D(x * frequency));
        denom += amplitude;

        frequency *= 2.0;
        amplitude *=0.5;
    }

    return output / denom;
}

float SimplexOctave2D(uint32_t octaves, float x, float y, float frequency, float amplitude){
    float output = 0;
    float denom = 0;

    for(int i=0;i < octaves; i++)
    {
        output += (amplitude * SimplexNoise2D(x * frequency, y * frequency));
        denom += amplitude;

        frequency *= 2.0;
        amplitude *=0.5;
    }

    return output / denom;
}

float sinWithRange(float value, float minV, float range){
    return ((fabs(sin(value)) + 1) / 2 * range) + minV;
}

float cosWithRange(float value, float minV, float range){
    return ((fabs(cos(value)) + 1) / 2 * range) + minV;
}

float* computeIcosahedronVertices(float radius)
{
    const float H_ANGLE = ((float)M_PI / 180) * 72;    // 72 degree = 360 / 5
    const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

    float *vertices = (float *)AllocateMemory(12 * 3, sizeof(float));    // 12 vertices
    int i1, i2;                             // indices
    float z, xy;                            // coords
    float hAngle1 = -((float)M_PI / 2) - H_ANGLE / 2;  // start from -126 deg at 2nd row
    float hAngle2 = -((float)M_PI / 2);                // start from -90 deg at 3rd row

    // the first top vertex (0, 0, r)
    vertices[0] = 0;
    vertices[1] = 0;
    vertices[2] = radius;

    // 10 vertices at 2nd and 3rd rows
    for(int i = 1; i <= 5; ++i)
    {
        i1 = i * 3;         // for 2nd row
        i2 = (i + 5) * 3;   // for 3rd row

        z = radius * sinf(V_ANGLE);             // elevaton
        xy = radius * cosf(V_ANGLE);

        vertices[i1] = xy * cosf(hAngle1);      // x
        vertices[i2] = xy * cosf(hAngle2);
        vertices[i1 + 1] = xy * sinf(hAngle1);  // x
        vertices[i2 + 1] = xy * sinf(hAngle2);
        vertices[i1 + 2] = z;                   // z
        vertices[i2 + 2] = -z;

        // next horizontal angles
        hAngle1 += H_ANGLE;
        hAngle2 += H_ANGLE;
    }

    // the last bottom vertex (0, 0, -r)
    i1 = 11 * 3;
    vertices[i1] = 0;
    vertices[i1 + 1] = 0;
    vertices[i1 + 2] = -radius;

    return vertices;
}

void addVertices3(Vertex3D* verts, const float v1[3], const float v2[3], const float v3[3], int current){
    verts[current + 0].position.x = v1[0];
    verts[current + 0].position.y = v1[1];
    verts[current + 0].position.z = v1[2];
    verts[current + 1].position.x = v2[0];
    verts[current + 1].position.y = v2[1];
    verts[current + 1].position.z = v2[2];
    verts[current + 2].position.x = v3[0];
    verts[current + 2].position.y = v3[1];
    verts[current + 2].position.z = v3[2];
}

void addIndices(uint32_t* indxs, int indx1, int indx2, int indx3, int current){

    indxs[current + 0] = indx1;
    indxs[current + 1] = indx2;
    indxs[current + 2] = indx3;
}

void addTexCoords3(Vertex3D* verts, const float t1[2], const float t2[2], const float t3[2], int current){
    verts[current + 0].texCoord.x = t1[0];
    verts[current + 0].texCoord.y = t1[1];
    verts[current + 1].texCoord.x = t2[0];
    verts[current + 1].texCoord.y = t2[1];
    verts[current + 2].texCoord.x = t3[0];
    verts[current + 2].texCoord.y = t3[1];
}

float computeScaleForLength(const float v[3], float length)
{
    // and normalize the vector then re-scale to new radius
    return length / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void computeHalfVertex(const float v1[3], const float v2[3], float length, float newV[3])
{
    newV[0] = v1[0] + v2[0];
    newV[1] = v1[1] + v2[1];
    newV[2] = v1[2] + v2[2];
    float scale = computeScaleForLength(newV, length);
    newV[0] *= scale;
    newV[1] *= scale;
    newV[2] *= scale;
}

///////////////////////////////////////////////////////////////////////////////
// find middle texcoords of 2 tex coords and return new coord (3rd param)
///////////////////////////////////////////////////////////////////////////////
void computeHalfTexCoord(const float t1[2], const float t2[2], float newT[2])
{
    newT[0] = (t1[0] + t2[0]) * 0.5f;
    newT[1] = (t1[1] + t2[1]) * 0.5f;
}

void subdivideVerticesFlat(vertexParam *vParam, indexParam *iParam, int subdivision, float radius){
    Vertex3D * tmpVerts;
    uint32_t* tmpIndxs;
    int indexCount;
    const float *v1, *v2, *v3;          // ptr to original vertices of a triangle
    const float *t1, *t2, *t3;          // ptr to original texcoords of a triangle
    float newV1[3], newV2[3], newV3[3]; // new vertex positions
    float newT1[2], newT2[2], newT3[2]; // new texture coords
    float normal[3];                    // new face normal
    unsigned int index = 0, curr = 0;             // new index value
    int i, j;

    // iteration
    for(i = 1; i <= subdivision; ++i)
    {
        tmpVerts = (Vertex3D *)AllocateMemory(vParam->num_verts, sizeof(Vertex3D));
        tmpIndxs = (uint32_t *)AllocateMemory(iParam->indexesSize, sizeof(uint32_t));
        memcpy(tmpVerts, vParam->vertices, vParam->num_verts * sizeof(Vertex3D));
        memcpy(tmpIndxs, iParam->indices, iParam->indexesSize * sizeof(uint32_t));

        FreeMemory(vParam->vertices);
        FreeMemory(iParam->indices);

        vParam->vertices = (Vertex3D *)AllocateMemory(vParam->num_verts * 4, sizeof(Vertex3D));
        iParam->indices = (uint32_t *)AllocateMemory(iParam->indexesSize * 4, sizeof(uint32_t));

        index = 0;
        curr = 0;
        indexCount = iParam->indexesSize;//(int)tmpIndices.size();

        for(j = 0; j < indexCount; j += 3)
        {
            // get 3 vertice and texcoords of a triangle
            v1 = (const float *)&tmpVerts[tmpIndxs[j]].position;
            v2 = (const float *)&tmpVerts[tmpIndxs[j + 1]].position;
            v3 = (const float *)&tmpVerts[tmpIndxs[j + 2]].position;
            t1 = (const float *)&tmpVerts[tmpIndxs[j]].texCoord;
            t2 = (const float *)&tmpVerts[tmpIndxs[j + 1]].texCoord;
            t3 = (const float *)&tmpVerts[tmpIndxs[j + 2]].texCoord;

            // get 3 new vertices by spliting half on each edge
            computeHalfVertex(v1, v2, radius, newV1);
            computeHalfVertex(v2, v3, radius, newV2);
            computeHalfVertex(v1, v3, radius, newV3);
            computeHalfTexCoord(t1, t2, newT1);
            computeHalfTexCoord(t2, t3, newT2);
            computeHalfTexCoord(t1, t3, newT3);

            // add 4 new triangles
            addVertices3(vParam->vertices, v1, newV1, newV3, curr);
            addTexCoords3(vParam->vertices, t1, newT1, newT3, curr);
            //computeFaceNormal(v1, newV1, newV3, normal);
            //addNormals(normal, normal, normal)
            addIndices(iParam->indices, index, index+1, index+2, curr);
            curr +=3;


            addVertices3(vParam->vertices, newV1, v2, newV2, curr);
            addTexCoords3(vParam->vertices, newT1, t2, newT2, curr);
            //computeFaceNormal(newV1, v2, newV2, normal);
            //addNormals(normal, normal, normal);
            addIndices(iParam->indices, index+3, index+4, index+5, curr);
            curr +=3;

            addVertices3(vParam->vertices, newV1, newV2, newV3, curr);
            addTexCoords3(vParam->vertices, newT1, newT2, newT3, curr);
            //computeFaceNormal(newV1, newV2, newV3, normal);
            //addNormals(normal, normal, normal);
            addIndices(iParam->indices, index+6, index+7, index+8, curr);
            curr +=3;

            addVertices3(vParam->vertices, newV3, newV2, v3, curr);
            addTexCoords3(vParam->vertices, newT3, newT2, t3, curr);
            //computeFaceNormal(newV3, newV2, v3, normal);
            //addNormals(normal, normal, normal);
            addIndices(iParam->indices, index+9, index+10, index+11, curr);
            curr +=3;

            // add new line indices per iteration
            //addSubLineIndices(index, index+1, index+4, index+5, index+11, index+9); //CCW

            // next index
            index += 12;
        }
        iParam->indexesSize = iParam->indexesSize * 4;
        vParam->num_verts = vParam->num_verts * 4;
        FreeMemory(tmpIndxs);
        FreeMemory(tmpVerts);
    }

    return;

}

int IcoSphereGenerator(vertexParam *vParam, indexParam *iParam,float radius)
{

    vParam->num_verts = 60;
    vParam->vertices = (Vertex3D *) AllocateMemory(60, sizeof(Vertex3D));
    iParam->indexesSize = 60;
    iParam->indices = (uint32_t *) AllocateMemory(60, sizeof(uint32_t));

    const float S_STEP = 186 / 2048.0f;     // horizontal texture step
    const float T_STEP = 322 / 1024.0f;     // vertical texture step

    // compute 12 vertices of icosahedron
    float* tmpVertices = computeIcosahedronVertices(radius);

    // clear memory of prev arrays

    const float *v0, *v1, *v2, *v3, *v4, *v11;          // vertex positions
    float n[3];                                         // face normal
    float t0[2], t1[2], t2[2], t3[2], t4[2], t11[2], tt[2];    // texCoords
    unsigned int index = 0, curr =0;

    // compute and add 20 tiangles of icosahedron first
    v0 = &tmpVertices[0];       // 1st vertex
    v11 = &tmpVertices[11 * 3]; // 12th vertex
    for(int i = 1; i <= 5; ++i)
    {
        // 4 vertices in the 2nd row
        v1 = &tmpVertices[i * 3];
        if(i < 5)
            v2 = &tmpVertices[(i + 1) * 3];
        else
            v2 = &tmpVertices[3];

        v3 = &tmpVertices[(i + 5) * 3];
        if((i + 5) < 10)
            v4 = &tmpVertices[(i + 6) * 3];
        else
            v4 = &tmpVertices[6 * 3];

        // texture coords
        t0[0] = (2 * i - 1) * S_STEP;   t0[1] = 0;
        t1[0] = (2 * i - 2) * S_STEP;   t1[1] = T_STEP;
        t2[0] = (2 * i - 0) * S_STEP;   t2[1] = T_STEP;
        t3[0] = (2 * i - 1) * S_STEP;   t3[1] = T_STEP * 2;
        t4[0] = (2 * i + 1) * S_STEP;   t4[1] = T_STEP * 2;
        t11[0]= 2 * i * S_STEP;         t11[1]= T_STEP * 3;

        // add a triangle in 1st row
        //Icosphere::computeFaceNormal(v0, v1, v2, n);
        addVertices3(vParam->vertices, v0, v1, v2, curr);
        //addNormals(n, n, n);
        addTexCoords3(vParam->vertices, t0, t1, t2, curr);
        addIndices(iParam->indices, index, index+1, index+2, curr);
        curr +=3;

        // add 2 triangles in 2nd row
        //Icosphere::computeFaceNormal(v1, v3, v2, n);
        addVertices3(vParam->vertices, v1, v3, v2, curr);
        //addNormals(n, n, n);
        addTexCoords3(vParam->vertices, t1, t3, t2, curr);
        addIndices(iParam->indices, index+3, index+4, index+5, curr);
        curr +=3;

        //Icosphere::computeFaceNormal(v2, v3, v4, n);
        addVertices3(vParam->vertices, v2, v3, v4, curr);
        //addNormals(n, n, n);
        addTexCoords3(vParam->vertices, t2, t3, t4, curr);
        addIndices(iParam->indices, index+6, index+7, index+8, curr);
        curr +=3;

        // add a triangle in 3rd row
        //Icosphere::computeFaceNormal(v3, v11, v4, n);
        addVertices3(vParam->vertices, v3, v11, v4, curr);
        //addNormals(n, n, n);
        addTexCoords3(vParam->vertices, t3, t11, t4, curr);
        addIndices(iParam->indices, index+9, index+10, index+11, curr);
        curr +=3;

        /*// add 6 edge lines per iteration
        //  i
        //  /   /   /   /   /       : (i, i+1)                              //
        // /__ /__ /__ /__ /__                                              //
        // \  /\  /\  /\  /\  /     : (i+3, i+4), (i+3, i+5), (i+4, i+5)    //
        //  \/__\/__\/__\/__\/__                                            //
        //   \   \   \   \   \      : (i+9,i+10), (i+9, i+11)               //
        //    \   \   \   \   \                                             //
        lineIndices.push_back(index);       // (i, i+1)
        lineIndices.push_back(index+1);       // (i, i+1)
        lineIndices.push_back(index+3);     // (i+3, i+4)
        lineIndices.push_back(index+4);
        lineIndices.push_back(index+3);     // (i+3, i+5)
        lineIndices.push_back(index+5);
        lineIndices.push_back(index+4);     // (i+4, i+5)
        lineIndices.push_back(index+5);
        lineIndices.push_back(index+9);     // (i+9, i+10)
        lineIndices.push_back(index+10);
        lineIndices.push_back(index+9);     // (i+9, i+11)
        lineIndices.push_back(index+11);

        // next index*/
        index += 12;
    }

    subdivideVerticesFlat(vParam, iParam, 2, radius);
    //free(tmpVertices);

    return 1;
}

float* getUnitPositiveX(unsigned int pointsPerRow)
{
    const float DEG2RAD = ((float)M_PI / 180.0f);

    float* vertices = (float *)AllocateMemory(pointsPerRow * pointsPerRow * 3, sizeof(float));
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along y-axis
    float a2;           // latitudinal angle
    float scale;

    int iter;
    // rotate latitudinal plane from 45 to -45 degrees along Z-axis
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            scale = computeScaleForLength(v, 1);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            iter = (i * pointsPerRow + j) * 3;

            vertices[iter] = v[0];
            vertices[iter + 1] = v[1];
            vertices[iter + 2] = v[2];

            // DEBUG
            //std::cout << "vertex: (" << v[0] << ", " << v[1] << ", " << v[2] << "), "
            //          << sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) << std::endl;
        }
    }

    return vertices;
}

void scaleVertex(float v[3], float scale)
{
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
}

void addVertices4(Vertex3D* verts, float v1[3], float v2[3], float v3[3], float v4[3], int current){
    verts[current + 0].position.x = v1[0];
    verts[current + 0].position.y = v1[1];
    verts[current + 0].position.z = v1[2];
    verts[current + 1].position.x = v2[0];
    verts[current + 1].position.y = v2[1];
    verts[current + 1].position.z = v2[2];
    verts[current + 2].position.x = v3[0];
    verts[current + 2].position.y = v3[1];
    verts[current + 2].position.z = v3[2];
    verts[current + 3].position.x = v4[0];
    verts[current + 3].position.y = v4[1];
    verts[current + 3].position.z = v4[2];
}

void addTexCoords4(Vertex3D* verts, float t1[2], float t2[2], float t3[2], float t4[2], int current){
    verts[current + 0].texCoord.x = t1[0];
    verts[current + 0].texCoord.y = t1[1];
    verts[current + 1].texCoord.x = t2[0];
    verts[current + 1].texCoord.y = t2[1];
    verts[current + 2].texCoord.x = t3[0];
    verts[current + 2].texCoord.y = t3[1];
    verts[current + 3].texCoord.x = t4[0];
    verts[current + 3].texCoord.y = t4[1];
}

void addTexCoords2(Vertex3D* verts, float t1[2], float t2[2], int current){
    verts[current + 0].texCoord.x = t1[0];
    verts[current + 0].texCoord.y = t1[1];
    verts[current + 1].texCoord.x = t2[0];
    verts[current + 1].texCoord.y = t2[1];
}

void addVertex(Vertex3D* verts, float x, float y, float z, int current)
{
    verts[current].position.x = x;
    verts[current].position.y = y;
    verts[current].position.z = z;
}

void addTexCoord(Vertex3D* verts, float s, float t, int current)
{
    verts[current].texCoord.x = s;
    verts[current].texCoord.y = t;
}

int Cubesphere(vertexParam *vParam, indexParam *iParam, float radius,int vertexCountPerRow){
    // generate unit-length verties in +X face
        float* unitVertices = getUnitPositiveX(vertexCountPerRow);

        vParam->num_verts = ((vertexCountPerRow - 1) * (vertexCountPerRow - 1)) * 4;
        vParam->vertices = (Vertex3D *) calloc(vParam->num_verts, sizeof(Vertex3D));
        iParam->indexesSize = ((vertexCountPerRow - 1) * (vertexCountPerRow - 1)) * 6;
        iParam->indices = (uint32_t *) calloc(iParam->indexesSize, sizeof(uint32_t));

        unsigned int k = 0, k1, k2, i1, i2, curr = 0, currI = 0; // indices
        float v1[3], v2[3], v3[3], v4[3];   // tmp vertices
        float t1[2], t2[2], t3[2], t4[2];   // texture coords
        float n[3];                         // normal vector

        // +X face
        for(unsigned int i = 0; i < vertexCountPerRow - 1; ++i)
        {
            k1 = i * vertexCountPerRow;              // index at curr row
            k2 = k1 + vertexCountPerRow;             // index at next row

            // vertical tex coords
            t1[1] = t3[1] = (float)i / (vertexCountPerRow - 1);
            t2[1] = t4[1] = (float)(i+1) / (vertexCountPerRow - 1);

            for(unsigned int j = 0; j < vertexCountPerRow - 1; ++j, ++k1, ++k2)
            {
                i1 = k1 * 3;
                i2 = k2 * 3;

                // 4 vertices of a quad
                // v1--v3
                // | / |
                // v2--v4
                v1[0] = unitVertices[i1];
                v1[1] = unitVertices[i1+1];
                v1[2] = unitVertices[i1+2];
                v2[0] = unitVertices[i2];
                v2[1] = unitVertices[i2+1];
                v2[2] = unitVertices[i2+2];
                v3[0] = unitVertices[i1+3];
                v3[1] = unitVertices[i1+4];
                v3[2] = unitVertices[i1+5];
                v4[0] = unitVertices[i2+3];
                v4[1] = unitVertices[i2+4];
                v4[2] = unitVertices[i2+5];

                // compute face nornal
                //Cubesphere::computeFaceNormal(v1, v2, v3, n);

                // resize vertices by sphere radius
                scaleVertex(v1, radius);
                scaleVertex(v2, radius);
                scaleVertex(v3, radius);
                scaleVertex(v4, radius);

                // compute horizontal tex coords
                t1[0] = t2[0] = (float)j / (vertexCountPerRow - 1);
                t3[0] = t4[0] = (float)(j+1) / (vertexCountPerRow - 1);

                // add 4 vertex attributes
                addVertices4(vParam->vertices, v1, v2, v3, v4, curr);
                //addNormals(n, n, n, n);
                addTexCoords4(vParam->vertices, t1, t2, t3, t4, curr);
                curr +=4;

                // add indices of 2 triangles
                addIndices(iParam->indices, k, k+1, k+2, currI);
                currI +=3;
                addIndices(iParam->indices, k+2, k+1, k+3, currI);
                currI +=3;

                // add line indices; top and left
                /*lineIndices.push_back(k);       // left
                lineIndices.push_back(k+1);
                lineIndices.push_back(k);       // top
                lineIndices.push_back(k+2);*/

                k += 4;     // next
            }
        }

        // array size and index for building next face
        unsigned int startIndex;                    // starting index for next face
        int vertexSize = (int)vParam->num_verts;      // vertex array size of +X face
        int indexSize = (int)iParam->indexesSize;        // index array size of +X face
        //int lineIndexSize = (int)lineIndices.size(); // line index size of +X face

        // build -X face by negating x and z values
        startIndex = vParam->num_verts;
        vParam->num_verts = vParam->num_verts + vertexSize;
        Vertex3D *verts = vParam->vertices = (Vertex3D *)realloc(vParam->vertices, vParam->num_verts * sizeof(Vertex3D));
        memset(vParam->vertices + startIndex, +  0, sizeof(Vertex3D) * vertexSize);
        for(int i = 0; i < vertexSize; i ++)
        {
            addVertex(verts, -verts[i].position.x, verts[i].position.y, -verts[i].position.z, curr);
            addTexCoord(verts, verts[i].texCoord.x, verts[i].texCoord.y, curr);
            //addNormal(-normals[i], normals[i+1], -normals[i+2]);
            curr ++;
        }
        iParam->indexesSize = iParam->indexesSize + indexSize;
        iParam->indices = (uint32_t *)realloc(iParam->indices, iParam->indexesSize * sizeof(uint32_t));
        for(int i = 0; i < indexSize; ++i)
        {
            iParam->indices[currI] = startIndex + iParam->indices[i];
            currI++;
        }

        /*for(int i = 0; i < lineIndexSize; i += 4)
        {
            // left and bottom lines
            lineIndices.push_back(startIndex + i);      // left
            lineIndices.push_back(startIndex + i + 1);
            lineIndices.push_back(startIndex + i + 1);  // bottom
            lineIndices.push_back(startIndex + i + 3);
        }*/

        // build +Y face by swapping x=>y, y=>-z, z=>-x
        startIndex = vParam->num_verts;
        vParam->num_verts = vParam->num_verts + vertexSize;
        verts = vParam->vertices = (Vertex3D *)realloc(vParam->vertices, vParam->num_verts * sizeof(Vertex3D));
        memset(vParam->vertices + startIndex, +  0, sizeof(Vertex3D) * vertexSize);
        for(int i = 0; i < vertexSize; i ++)
        {
            addVertex(verts, -verts[i].position.z, verts[i].position.x, -verts[i].position.y, curr);
            addTexCoord(verts, verts[i].texCoord.x, verts[i].texCoord.y, curr);
            //addNormal(-normals[i+2], normals[i], -normals[i+1]);
            curr ++;
        }
        iParam->indexesSize = iParam->indexesSize + indexSize;
        iParam->indices = (uint32_t *)realloc(iParam->indices, iParam->indexesSize * sizeof(uint32_t));
        for(int i = 0; i < indexSize; ++i)
        {
            iParam->indices[currI] = startIndex + iParam->indices[i];
            currI++;
        }

        /*for(int i = 0; i < lineIndexSize; ++i)
        {
            // top and left lines (same as +X)
            lineIndices.push_back(startIndex + lineIndices[i]);
        }*/

        // build -Y face by swapping x=>-y, y=>z, z=>-x
        startIndex = vParam->num_verts;
        vParam->num_verts = vParam->num_verts + vertexSize;
        verts = vParam->vertices = (Vertex3D *)realloc(vParam->vertices, vParam->num_verts * sizeof(Vertex3D));
        memset(vParam->vertices + startIndex, +  0, sizeof(Vertex3D) * vertexSize);
        for(int i = 0; i < vertexSize; i ++)
        {
            addVertex(verts, -verts[i].position.z, -verts[i].position.x, verts[i].position.y, curr);
            addTexCoord(verts, verts[i].texCoord.x, verts[i].texCoord.y, curr);
            //addNormal(-normals[i+2], -normals[i], normals[i+1]);
            curr ++;
        }
        iParam->indexesSize = iParam->indexesSize + indexSize;
        iParam->indices = (uint32_t *)realloc(iParam->indices, iParam->indexesSize * sizeof(uint32_t));
        for(int i = 0; i < indexSize; ++i)
        {
            iParam->indices[currI] = startIndex + iParam->indices[i];
            currI++;
        }

        /*for(int i = 0; i < lineIndexSize; i += 4)
        {
            // top and right lines
            lineIndices.push_back(startIndex + i);      // top
            lineIndices.push_back(startIndex + i + 2);
            lineIndices.push_back(startIndex + 2 + i);  // right
            lineIndices.push_back(startIndex + 3 + i);
        }*/

        // build +Z face by swapping x=>z, z=>-x
        startIndex = vParam->num_verts;
        vParam->num_verts = vParam->num_verts + vertexSize;
        verts = vParam->vertices = (Vertex3D *)realloc(vParam->vertices, vParam->num_verts * sizeof(Vertex3D));
        memset(vParam->vertices + startIndex, +  0, sizeof(Vertex3D) * vertexSize);
        for(int i = 0; i < vertexSize; i ++)
        {
            addVertex(verts, -verts[i].position.z, verts[i].position.y, verts[i].position.x, curr);
            addTexCoord(verts, verts[i].texCoord.x, verts[i].texCoord.y, curr);
            //addNormal(-normals[i+2], normals[i+1], normals[i]);
            curr ++;
        }
        iParam->indexesSize = iParam->indexesSize + indexSize;
        iParam->indices = (uint32_t *)realloc(iParam->indices, iParam->indexesSize * sizeof(uint32_t));
        for(int i = 0; i < indexSize; ++i)
        {
            iParam->indices[currI] = startIndex + iParam->indices[i];
            currI++;
        }

        /*for(int i = 0; i < lineIndexSize; ++i)
        {
            // top and left lines (same as +X)
            lineIndices.push_back(startIndex + lineIndices[i]);
        }*/

        // build -Z face by swapping x=>-z, z=>x
        startIndex = vParam->num_verts;
        vParam->num_verts = vParam->num_verts + vertexSize;
        verts = vParam->vertices = (Vertex3D *)realloc(vParam->vertices, vParam->num_verts * sizeof(Vertex3D));
        memset(vParam->vertices + startIndex, +  0, sizeof(Vertex3D) * vertexSize);
        for(int i = 0; i < vertexSize; i ++)
        {
            addVertex(verts, verts[i].position.z, verts[i].position.y, -verts[i].position.x, curr);
            addTexCoord(verts, verts[i].texCoord.x, verts[i].texCoord.y, curr);
            //addNormal(normals[i+2], normals[i+1], -normals[i]);
            curr ++;
        }
        iParam->indexesSize = iParam->indexesSize + indexSize;
        iParam->indices = (uint32_t *)realloc(iParam->indices, iParam->indexesSize * sizeof(uint32_t));
        for(int i = 0; i < indexSize; ++i)
        {
            iParam->indices[currI] = startIndex + iParam->indices[i];
            currI++;
        }

        /*for(int i = 0; i < lineIndexSize; i += 4)
        {
            // left and bottom lines
            lineIndices.push_back(startIndex + i);      // left
            lineIndices.push_back(startIndex + i + 1);
            lineIndices.push_back(startIndex + i + 1);  // bottom
            lineIndices.push_back(startIndex + i + 3);
        }*/

        return 1;

}

int SphereGenerator3D(vertexParam *vParam, indexParam *iParam,float radius, int stackCount, int sectorCount){
    float x, y, z, xz;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = (2 * (float)M_PI) / sectorCount;
    float stackStep = ((float)M_PI / stackCount);
    float sectorAngle, stackAngle;

    vParam->num_verts = (stackCount + 1) * (sectorCount + 1);

    vParam->vertices = (Vertex3D *) AllocateMemory(vParam->num_verts, sizeof(Vertex3D));

    int vIter = 0;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = ((float)M_PI / 2) - i * stackStep;        // starting from pi/2 to -pi/2
        xz = radius * -cosf(stackAngle);             // r * cos(u)
        y = radius * -sinf(stackAngle);              // r * sin(u)

        Vertex3D *verts = vParam->vertices;

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            vIter = i * sectorCount + (i > 0 ? j + i : j);

            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xz * cosf(sectorAngle);             // r * cos(u) * cos(v)
            z = xz * sinf(sectorAngle);             // r * cos(u) * sin(v)
            verts[vIter].position = (vec3){x, y, z};

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            verts[vIter].normal = (vec3){nx, ny, nz};

            // vertex tex coord (s, t) range between [0, 1]

            s = (float)j / stackCount;
            t = (float)i / sectorCount;

            verts[vIter].texCoord = (vec2){s, t};
        }
    }

    iParam->indexesSize = (stackCount * sectorCount) * 6 + 6;

    iParam->indices = (uint32_t *) AllocateMemory(iParam->indexesSize, sizeof(uint32_t));

    int k1, k2;
    int it = 0, tt = 0;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1 ;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1

            iParam->indices[it] = k1 + 1;
            iParam->indices[it + 1] = k2;
            iParam->indices[it + 2] = k1;
            it +=3;

            // k1+1 => k2 => k2+1
            if(i != (stackCount-1))
            {
                iParam->indices[it] = k2 + 1;
                iParam->indices[it + 1] = k2;
                iParam->indices[it + 2] = k1 + 1;
                it +=3;
            }
        }
    }

    return it;
}

void ConeGenerator(vertexParam *vParam, indexParam *iParam, const float height, int stackCount, int sectorCount) {

    vec3 pos = {0,0,0};

    float x, z;                              // vertex positio

    float angIncSector = ((float)2 * M_PI) / sectorCount;
    float angIncStack = ((float)2 * M_PI) / stackCount;
    float heigInc = height / stackCount;
    float sectorAngle, stackAngle;

    vParam->num_verts = ((stackCount + 1) * (sectorCount + 1)) * 2;

    vParam->vertices = (Vertex3D *) AllocateMemory(vParam->num_verts, sizeof(Vertex3D));

    Vertex3D *verts = vParam->vertices;

    int vIter = 0;
    int mulcount = (stackCount + 1) * (sectorCount + 1);
    // calculate points around directrix
    for (int i = 0; i <= stackCount; ++i) {

        stackAngle = i * angIncSector;        // starting from pi/2 to -pi/2

        for(int j = 0; j <= sectorCount; ++j)
        {
            vIter = i * sectorCount + (i > 0 ? j + i : j);

            sectorAngle = j * angIncStack;           // starting from 0 to 2pi

            x = cos(sectorAngle) * stackAngle;             // r * cos(u) * cos(v)
            z = sin(sectorAngle) * stackAngle;              // r * sin(u)

            vec3 posit;
            posit.x = x;
            posit.y = pos.y;
            posit.z = z;
            verts[vIter].position = posit;
            verts[vIter].texCoord = (vec2){(float)i / stackCount, (float)j / sectorCount};
        }
    }

    for (int i = 0; i <= stackCount; ++i) {

        stackAngle = i * angIncSector;        // starting from pi/2 to -pi/2

        for(int j = 0; j <= sectorCount; ++j)
        {
            vIter = mulcount + i * sectorCount + (i > 0 ? j + i : j);

            sectorAngle = j * angIncStack;           // starting from 0 to 2pi

            x = -cos(sectorAngle) * stackAngle;             // r * cos(u) * cos(v)
            z = -sin(sectorAngle) * stackAngle;              // r * sin(u)

            vec3 posit;
            posit.x = x;
            posit.y = pos.y + i * heigInc - height;
            posit.z = z;
            verts[vIter].position = posit;
            verts[vIter].texCoord = (vec2){(float)i / stackCount, (float)j / sectorCount};
        }
    }

    iParam->indexesSize = (stackCount * sectorCount) * 2 * 6;

    iParam->indices = (uint32_t *) AllocateMemory(iParam->indexesSize, sizeof(uint32_t));

    int k1, k2;
    int it = 0, tt = 0;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1 ;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1

            iParam->indices[it] = k1 + 1;
            iParam->indices[it + 1] = k2;
            iParam->indices[it + 2] = k1;
            it +=3;

            // k1+1 => k2 => k2+1
            iParam->indices[it] = k1 + 1;
            iParam->indices[it + 1] = k2;
            iParam->indices[it + 2] = k2 + 1;
            it +=3;
        }
    }

    int pl = mulcount;

    for(int i = 0; i < stackCount; ++i)
    {
        k1 = pl + i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1 ;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1

            iParam->indices[it] = k1 + 1;
            iParam->indices[it + 1] = k2;
            iParam->indices[it + 2] = k1;
            it +=3;

            // k1+1 => k2 => k2+1
            iParam->indices[it] = k2 + 1;
            iParam->indices[it + 1] = k2;
            iParam->indices[it + 2] = k1 + 1;
            it +=3;
        }
    }

}

void InitPlane3D(vertexParam *vParam, indexParam *iParam, int stackCount, int sectorCount){

    vec3 pos = {0 , 0, 0};
    vec3 col = {0.3 , 0.1, 0.11};

    vParam->num_verts = (stackCount + 1) * (sectorCount + 1);

    vParam->vertices = (Vertex3D *) AllocateMemory(vParam->num_verts, sizeof(Vertex3D));

    int vIter = 0;

    Vertex3D *verts = vParam->vertices;

    const float wx = 2.0f;
    const float wy = 2.0f;

    uint32_t vertex_step = 1.0;
    for(int x=0; x <= stackCount;x++){
        for(int z=0; z <= sectorCount;z++){

            vIter = x * sectorCount + (x > 0 ? z + x : z);

            verts[vIter].position.x = (x * wx + wx / 2.0f - (float)stackCount * wx / 2.0f) * vertex_step;
            verts[vIter].position.y = 0;
            verts[vIter].position.z = (z * wy + wy / 2.0f - (float)sectorCount * wy / 2.0f) * vertex_step;
            verts[vIter].normal = vec3_f(0, 1, 0);


            verts[vIter].texCoord = vec2_f((float)x / sectorCount, (float)z / stackCount);

        }
    }

    iParam->indexesSize = (stackCount * sectorCount) * 6 + 6;

    iParam->indices = (uint32_t *) AllocateMemory(iParam->indexesSize, sizeof(uint32_t));

    int k1, k2, it = 0, tt = 0;

    for(int i=0; i < stackCount;i++){
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 +  sectorCount + 1 ;      // beginning of next stack
        for(int j=0; j < sectorCount; ++j, ++k1, ++k2){

            iParam->indices[it] = k1;
            iParam->indices[it + 1] = k2;
            iParam->indices[it + 2] = k1 + 1;
            it +=3;

            iParam->indices[it] = k2;
            iParam->indices[it + 1] = k2 + 1;
            iParam->indices[it + 2] = k1 + 1;

            it +=3;
        }
    }
}

char *ToolsMakeString(char *s1, char *s2)
{

    int len = strlen(s1);
    int len2 = strlen(s2);

    char* out = AllocateMemoryP(len + len2 + 1, sizeof(char), &engine);

    memcpy(out, s1, len);
    memcpy(out + len, s2, len2);
    memset(out + len + len2, 0, 1);

    return out;
}

void ToolsAddStrings(char *out, int buff_size, char *s1, char *s2){

    char temp[buff_size];

    memcpy(temp, s1, buff_size);

    memset(out, 0, buff_size);

    int len = strlen(temp);
    int len2 = strlen(s2);

    memcpy(out, temp, len);
    memcpy(out + len, s2, len2);

    memset(out + len + len2, 0, 1);

}

bool ToolsCmpStrings(char *in, char *s1){

    char *cursor = in;

    int len = strlen(s1) - 1, iter = 0;;

    while(*cursor == s1[iter] && iter!=len)
    {
        cursor++;
        iter++;
    }

    if(iter == len)
        return true;

    return false;
}

int ToolsStr32BitLength(uint32_t *text)
{
    int size = 0;

    uint32_t *point = text;

    while(*point != 0)
    {
        size ++;
        point++;
    }

    return size;
}

void* ToolsLoadImageFromFile(size_t* len, char *filepath)
{
    FILE* fd;
    int size;

    fd = fopen(filepath, "r");
    if (fd == NULL) {
        printf("File Not Found!\n");
        return NULL;
    }


    fseek(fd, 0L, SEEK_END);
    size = ftell(fd);

    char *buff = (char *)AllocateMemoryP(size, sizeof(char), &engine);

    fseek(fd, 0L, SEEK_SET);

    fread(buff, sizeof(char), size, fd);

    fflush(fd);

    fclose(fd);

    *len = size;

    return buff;
}

int ToolsUInt32ToString(char *dest, const uint32_t *src)
{
    uint32_t size = ToolsStr32BitLength((uint32_t *)src);

    bool find = false;
    int iter = 0;
    for(int i=0; i < size; i++)
    {
        find = false;

        for(int j=0; j < 66; j++)
        {
            if(fontIndexes[j].FindLetter == src[i])
            {
                dest[iter] = fontIndexes[j].IndexLetter >> 8;
                dest[iter + 1] = fontIndexes[j].IndexLetter & 0xFF;
                find = true;
                break;
            }
        }

        if(!find){
            dest[iter] = src[i];
            iter ++;
        }else
            iter +=2;
    }

    dest[iter] = 0;

    return iter;
}

int ToolsTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end)
{
    static const char lengths[32] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0 };
    static const int masks[]  = { 0x00, 0x7f, 0x1f, 0x0f, 0x07 };
    static const uint32_t mins[] = { 0x400000, 0, 0x80, 0x800, 0x10000 };
    static const int shiftc[] = { 0, 18, 12, 6, 0 };
    static const int shifte[] = { 0, 6, 4, 2, 0 };
    int len = lengths[*(const unsigned char*)in_text >> 3];
    int wanted = len + (len ? 0 : 1);

    if (in_text_end == NULL)
        in_text_end = in_text + wanted; // Max length, nulls will be taken into account.

    // Copy at most 'len' bytes, stop copying at 0 or past in_text_end. Branch predictor does a good job here,
    // so it is fast even with excessive branching.
    unsigned char s[4];
    s[0] = in_text + 0 < in_text_end ? in_text[0] : 0;
    s[1] = in_text + 1 < in_text_end ? in_text[1] : 0;
    s[2] = in_text + 2 < in_text_end ? in_text[2] : 0;
    s[3] = in_text + 3 < in_text_end ? in_text[3] : 0;

    // Assume a four-byte character and load four bytes. Unused bits are shifted out.
    *out_char  = (uint32_t)(s[0] & masks[len]) << 18;
    *out_char |= (uint32_t)(s[1] & 0x3f) << 12;
    *out_char |= (uint32_t)(s[2] & 0x3f) <<  6;
    *out_char |= (uint32_t)(s[3] & 0x3f) <<  0;
    *out_char >>= shiftc[len];

    // Accumulate the various error conditions.
    int e = 0;
    e  = (*out_char < mins[len]) << 6; // non-canonical encoding
    e |= ((*out_char >> 11) == 0x1b) << 7;  // surrogate half?
    e |= (*out_char > TIGOR_UNICODE_CODEPOINT_MAX) << 8;  // out of range?
    e |= (s[1] & 0xc0) >> 2;
    e |= (s[2] & 0xc0) >> 4;
    e |= (s[3]       ) >> 6;
    e ^= 0x2a; // top two bits of each tail byte correct?
    e >>= shifte[len];

    if (e)
    {
        // No bytes are consumed when *in_text == 0 || in_text == in_text_end.
        // One byte is consumed in case of invalid first byte of in_text.
        // All available bytes (at most `len` bytes) are consumed on incomplete/invalid second to last bytes.
        // Invalid or incomplete input may consume less bytes than wanted, therefore every byte has to be inspected in s.
        wanted = e_min(wanted, !!s[0] + !!s[1] + !!s[2] + !!s[3]);
        *out_char = TIGOR_UNICODE_CODEPOINT_INVALID;
    }

    return wanted;
}

int ToolsTextStrFromUtf8(uint32_t* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_text_remaining)
{
    uint32_t* buf_out = buf;
    uint32_t* buf_end = buf + buf_size;
    while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) && *in_text)
    {
        unsigned int c;
        in_text += ToolsTextCharFromUtf8(&c, in_text, in_text_end);
        *buf_out++ = (uint32_t)c;
    }
    *buf_out = 0;
    if (in_text_remaining)
        *in_text_remaining = in_text;
    return (int)(buf_out - buf);
}

int ToolsStringToUInt32(uint32_t *dest, const char *src)
{
    uint32_t size = strlen(src);

    bool find = false;
    int iter = 0;
    for(int i=0; i < size; i++)
    {
        find = false;
        if(src[i] < 0)
        {
            for(int j=0; j < 66; j++)
            {
                char temp[2];
                temp[0] = src[i];
                temp[1] = src[i + 1];
                uint16_t t_value = *(uint16_t *)temp;
                if(fontIndexes[j].IndexLetter == t_value)
                {
                    dest[iter] = fontIndexes[j].FindLetter;
                    find = true;
                    break;
                }
            }

            i ++;
        }

        if(!find)
            dest[iter] = src[i];

        iter++;
    }

    dest[iter] = 0;

    return iter;
}
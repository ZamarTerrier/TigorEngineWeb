#include "e_tools.h"
#include "e_math.h"

#include <math.h>
#include <string.h>

#include "e_memory.h"

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
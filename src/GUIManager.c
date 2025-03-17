#include "GUIManager.h"

#include <stdio.h>
#include <fcntl.h>

#define STB_TRUETYPE_IMPLEMENTATION

#include "stb_truetype.h"

#include "engine.h"
#include "TigorGUI.h"

#include "e_camera.h"

#include "default_font.h"

#include "e_memory.h"

#include "e_tools.h"
#include "e_math.h"

const char *guiVertexShaderSource = 
      "attribute vec2 position;   \n"
      "attribute vec4 color;   \n"
      "attribute vec2 textCoord;\n"
      "uniform mat4 u_model; \n"
      "uniform mat4 u_view; \n"
      "uniform mat4 u_proj; \n"
      "varying vec2 vTextureCoords;\n"
      "varying vec4 vFragColor;\n"
      "void main()                 \n"
      "{                           \n"
      "   gl_Position = u_proj * u_view * vec4(position, 0.0, 1.0); \n"
      "   vTextureCoords = textCoord;\n"
      "   vFragColor = color;\n"
      "}                           \n";
const char *guiFragmentShaderSource = 
#ifdef EMSCRIPTEN
      "precision mediump float;                   \n"
#endif
      "uniform sampler2D u_texture;\n"
      "varying vec2 vTextureCoords;\n"
      "varying vec4 vFragColor;\n"
      "void main()                                \n"
      "{                                          \n"
      "  vec4 color =  texture2D(u_texture, vTextureCoords); \n"
      "  gl_FragColor = vec4(vFragColor.rgb, color.a * vFragColor.a); \n"
      "}                                          \n";


double GUIRsqrt(double x)          { return 1.0 / sqrt(x); }

// Helper Macros
#ifndef GUI_ASSERT
#include <assert.h>
#define GUI_ASSERT(_EXPR)            assert(_EXPR)                               // You can override the default assert handler by editing imconfig.h
#endif

#define GUI_NORMALIZE2F_OVER_ZERO(VX,VY)     { float d2 = VX*VX + VY*VY; if (d2 > 0.0f) { float inv_len = GUIRsqrt(d2); VX *= inv_len; VY *= inv_len; } } (void)0

// ImDrawList: Lookup table size for adaptive arc drawing, cover full circle.
#ifndef GUI_DRAWLIST_ARCFAST_TABLE_SIZE
#define GUI_DRAWLIST_ARCFAST_TABLE_SIZE                          48 // Number of samples in lookup table.
#endif
#define GUI_DRAWLIST_ARCFAST_SAMPLE_MAX                          GUI_DRAWLIST_ARCFAST_TABLE_SIZE // Sample index _PathArcToFastEx() for 360 angle.

#define GUI_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))     // Size of a static C-style array. Don't use on pointers!

#define GUI_ROUNDUP_TO_EVEN(_V)                                  ((((_V) + 1) / 2) * 2)
#define GUI_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN                     4
#define GUI_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX                     512
#define GUI_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(_RAD,_MAXERROR)    clamp(GUI_ROUNDUP_TO_EVEN((int)ceil(M_PI / acos(1 - e_min((_MAXERROR), (_RAD)) / (_RAD)))), GUI_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN, GUI_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX)
#define GUI_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(_N,_MAXERROR)    ((_MAXERROR) / (1 - cos(M_PI / e_max((float)(_N), M_PI))))

#define MAX_VERTEX_SIZE 2048
#define MAX_INDEX_SIZE 4096

GUIManager gui;

extern TEngine engine;

int GUIFontResizer = 7;

int GUICurrVertexMaxCount = 0, GUICurrIndexMaxCount = 0;

vec2 ArcFastVtx[GUI_DRAWLIST_ARCFAST_TABLE_SIZE];
uint8_t CircleSegmentCounts[64];
float CircleSegmentMaxError = 0.3f;
float ArcFastRadiusCutoff;
vec2 TexUvWhitePixel;

int _CalcCircleAutoSegmentCount(float radius)
{
    // Automatic segment count
    const int radius_idx = (int)(radius + 0.999999f); // ceil to never reduce accuracy
    if (radius_idx >= 0 && radius_idx < GUI_ARRAYSIZE(CircleSegmentCounts))
        return CircleSegmentCounts[radius_idx]; // Use cached value
    else
        return GUI_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(radius, CircleSegmentMaxError);
}

void _PathArcToFastEx(const vec2 center, float radius, int a_min_sample, int a_max_sample, int a_step)
{
    if (radius < 0.5f)
    {
        gui._Path[gui._Path_Size] = center;
        gui._Path_Size ++;
        return;
    }

    // Calculate arc auto segment step size
    if (a_step <= 0)
        a_step = GUI_DRAWLIST_ARCFAST_SAMPLE_MAX / _CalcCircleAutoSegmentCount(radius);

    // Make sure we never do steps larger than one quarter of the circle
    a_step = clamp(a_step, 1, GUI_DRAWLIST_ARCFAST_TABLE_SIZE / 4);

    const int sample_range = abs(a_max_sample - a_min_sample);
    const int a_next_step = a_step;

    int samples = sample_range + 1;
    bool extra_max_sample = false;
    if (a_step > 1)
    {
        samples            = sample_range / a_step + 1;
        const int overstep = sample_range % a_step;

        if (overstep > 0)
        {
            extra_max_sample = true;
            samples++;

            // When we have overstep to avoid awkwardly looking one long line and one tiny one at the end,
            // distribute first step range evenly between them by reducing first step size.
            if (sample_range > 0)
                a_step -= (a_step - overstep) / 2;
        }
    }

    int sample_index = a_min_sample;
    if (sample_index < 0 || sample_index >= GUI_DRAWLIST_ARCFAST_SAMPLE_MAX)
    {
        sample_index = sample_index % GUI_DRAWLIST_ARCFAST_SAMPLE_MAX;
        if (sample_index < 0)
            sample_index += GUI_DRAWLIST_ARCFAST_SAMPLE_MAX;
    }

    if (a_max_sample >= a_min_sample)
    {
        for (int a = a_min_sample; a <= a_max_sample; a += a_step, sample_index += a_step, a_step = a_next_step)
        {
            // a_step is clamped to IM_DRAWLIST_ARCFAST_SAMPLE_MAX, so we have guaranteed that it will not wrap over range twice or more
            if (sample_index >= GUI_DRAWLIST_ARCFAST_SAMPLE_MAX)
                sample_index -= GUI_DRAWLIST_ARCFAST_SAMPLE_MAX;

            const vec2 s = ArcFastVtx[sample_index];
            gui._Path[gui._Path_Size].x = center.x + s.x * radius;
            gui._Path[gui._Path_Size].y = center.y + s.y * radius;
            gui._Path_Size ++;
        }
    }
    else
    {
        for (int a = a_min_sample; a >= a_max_sample; a -= a_step, sample_index -= a_step, a_step = a_next_step)
        {
            // a_step is clamped to IM_DRAWLIST_ARCFAST_SAMPLE_MAX, so we have guaranteed that it will not wrap over range twice or more
            if (sample_index < 0)
                sample_index += GUI_DRAWLIST_ARCFAST_SAMPLE_MAX;

            const vec2 s = ArcFastVtx[sample_index];
            gui._Path[gui._Path_Size].x = center.x + s.x * radius;
            gui._Path[gui._Path_Size].y = center.y + s.y * radius;            
            gui._Path_Size ++;
        }
    }

    if (extra_max_sample)
    {
        int normalized_max_sample = a_max_sample % GUI_DRAWLIST_ARCFAST_SAMPLE_MAX;
        if (normalized_max_sample < 0)
            normalized_max_sample += GUI_DRAWLIST_ARCFAST_SAMPLE_MAX;

        const vec2 s = ArcFastVtx[sample_index];
        gui._Path[gui._Path_Size].x = center.x + s.x * radius;
        gui._Path[gui._Path_Size].y = center.y + s.y * radius;        
        gui._Path_Size ++;
    }
}

void _PathArcToN(const vec2 center, float radius, float a_min, float a_max, int num_segments)
{
    if (radius < 0.5f)
    {
        gui._Path[gui._Path_Size] = center;
        gui._Path_Size++;
        return;
    }

    // Note that we are adding a point at both a_min and a_max.
    // If you are trying to draw a full closed circle you don't want the overlapping points!
    for (int i = 0; i <= num_segments; i++)
    {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        
        gui._Path[gui._Path_Size] = vec2_f(center.x + cos(a) * radius, center.y + sin(a) * radius);
        gui._Path_Size++;
    }
}

void PathArcTo(const vec2 center, float radius, float a_min, float a_max, int num_segments)
{
    if (radius < 0.5f)
    {
        gui._Path[gui._Path_Size] = center;
        gui._Path_Size++;
        return;
    }

    if (num_segments > 0)
    {
        _PathArcToN(center, radius, a_min, a_max, num_segments);
        return;
    }

    // Automatic segment count
    if (radius <= ArcFastRadiusCutoff)
    {
        const bool a_is_reverse = a_max < a_min;

        // We are going to use precomputed values for mid samples.
        // Determine first and last sample in lookup table that belong to the arc.
        const float a_min_sample_f = GUI_DRAWLIST_ARCFAST_SAMPLE_MAX * a_min / (M_PI * 2.0f);
        const float a_max_sample_f = GUI_DRAWLIST_ARCFAST_SAMPLE_MAX * a_max / (M_PI * 2.0f);

        const int a_min_sample = a_is_reverse ? (int)floor(a_min_sample_f) : (int)ceil(a_min_sample_f);
        const int a_max_sample = a_is_reverse ? (int)ceil(a_max_sample_f) : (int)floor(a_max_sample_f);
        const int a_mid_samples = a_is_reverse ? e_max(a_min_sample - a_max_sample, 0) : e_max(a_max_sample - a_min_sample, 0);

        const float a_min_segment_angle = a_min_sample * M_PI * 2.0f / GUI_DRAWLIST_ARCFAST_SAMPLE_MAX;
        const float a_max_segment_angle = a_max_sample * M_PI * 2.0f / GUI_DRAWLIST_ARCFAST_SAMPLE_MAX;
        const bool a_emit_start = fabsf(a_min_segment_angle - a_min) >= 1e-5f;
        const bool a_emit_end = fabsf(a_max - a_max_segment_angle) >= 1e-5f;

        if (a_emit_start){
            gui._Path[gui._Path_Size] = vec2_f(center.x + cos(a_min) * radius, center.y + sin(a_min) * radius);
            gui._Path_Size++;
        }
        if (a_mid_samples > 0)
            _PathArcToFastEx(center, radius, a_min_sample, a_max_sample, 0);
        if (a_emit_end){
            gui._Path[gui._Path_Size] = vec2_f(center.x + cos(a_min) * radius, center.y + sin(a_min) * radius);
            gui._Path_Size++;
        }
    }
    else
    {
        const float arc_length = fabsf(a_max - a_min);
        const int circle_segment_count = _CalcCircleAutoSegmentCount(radius);
        const int arc_segment_count = e_max((int)ceil(circle_segment_count * arc_length / (M_PI * 2.0f)), (int)(2.0f * M_PI / arc_length));
        _PathArcToN(center, radius, a_min, a_max, arc_segment_count);
    }
}

void PathArcToFast(vec2 center, float radius, int a_min_of_12, int a_max_of_12)
{
    if (radius < 0.5f)
    {
        gui._Path[gui._Path_Size] = center;
        gui._Path_Size++;
        return;
    }
    _PathArcToFastEx(center, radius, a_min_of_12 * GUI_DRAWLIST_ARCFAST_SAMPLE_MAX / 12, a_max_of_12 * GUI_DRAWLIST_ARCFAST_SAMPLE_MAX / 12, 0);
}

void PathEllipticalArcTo(const vec2 center, const vec2 radius, float rot, float a_min, float a_max, int num_segments)
{
    if (num_segments <= 0)
        num_segments = _CalcCircleAutoSegmentCount(e_max(radius.x, radius.y)); // A bit pessimistic, maybe there's a better computation to do here.

    const float cos_rot = cos(rot);
    const float sin_rot = sin(rot);
    for (int i = 0; i <= num_segments; i++)
    {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        vec2 point = {cos(a) * radius.x, sin(a) * radius.y};
        vec2 rel =  {(point.x * cos_rot) - (point.y * sin_rot), (point.x * sin_rot) + (point.y * cos_rot)};
        point.x = rel.x + center.x;
        point.y = rel.y + center.y;
        
        gui._Path[gui._Path_Size] = point;
        gui._Path_Size++;
    }
}

GUIObj *GUIManagerAddObject(){

    if(gui.draw_list == NULL)
        gui.draw_list = calloc(1, sizeof(ChildStack));

    GUIObj *object = calloc(1, sizeof(GUIObj));

    if(gui.draw_list->node == NULL){
        gui.draw_list->next = calloc(1, sizeof(ChildStack));
        gui.draw_list->node = object;
    }
    else{

        ChildStack *child = gui.draw_list;

        while(child->next != NULL)
        {
            child = child->next;
        }

        child->next = calloc(1, sizeof(ChildStack));
        child->node = object;
    }

    return object;
}

uint32_t GUIManagerObjCount(){
    ChildStack *child = gui.draw_list;

    uint32_t counter = 0;

    if(child == NULL)
        return counter;
    
    while(child->next != NULL){
        counter++;

        child = child->next;
    }

    return counter;   
}

void GUIManagerGetVertexCount(uint32_t *vertCount, uint32_t *indxCount){

    ChildStack *child = gui.draw_list;
    
    GUIObj *obj = NULL;

    *vertCount = 0;
    *indxCount = 0;

    uint32_t counter = 0;
    while(child != NULL){
        obj = child->node;

        if(obj != NULL){
            *vertCount += obj->vert_count;
            *indxCount += obj->indx_count;
        }

        child = child->next;
    }
}

void GUIManagerCopyVertex(uint32_t vCount, uint32_t iCount){

    ChildStack *child = gui.draw_list;
    
    GUIObj *obj = NULL;

    uint32_t counter = 0;   

    uint32_t vertCount, indxCount;

    GUIManagerGetVertexCount(&vertCount, &indxCount);

    if(GUICurrVertexMaxCount < vertCount){
        FreeMemory(gui.go.graphObj.shapes[0].vParam.vertices);
        FreeMemory(gui.go.graphObj.shapes[0].iParam.indices);

        while(GUICurrVertexMaxCount < vertCount)
            GUICurrVertexMaxCount = GUICurrVertexMaxCount << 1;

        GUICurrIndexMaxCount =  GUICurrVertexMaxCount << 1;
        
        gui.go.graphObj.shapes[0].vParam.vertices = AllocateMemory(GUICurrVertexMaxCount, sizeof(Vertex2DGUI));
        gui.go.graphObj.shapes[0].iParam.indices = AllocateMemory(GUICurrIndexMaxCount, sizeof(uint32_t));
    }

   
    char *dataV = (char *)gui.go.graphObj.shapes[0].vParam.vertices;
    char *dataI = (char *)gui.go.graphObj.shapes[0].iParam.indices;
    
    gui.go.graphObj.shapes[0].vParam.num_verts = vertCount;
    gui.go.graphObj.shapes[0].iParam.indexesSize = indxCount;

    memset(dataV, 0, GUICurrVertexMaxCount);
    memset(dataI, 0, GUICurrIndexMaxCount);

    while(child != NULL){
        obj = child->node;

        if(obj != NULL){
            
            memcpy(dataV, obj->points, (size_t) sizeof(Vertex2DGUI) * obj->vert_count);
            memcpy(dataI, obj->indeces, (size_t) sizeof(uint32_t) * obj->indx_count);

            dataV += sizeof(Vertex2DGUI) * obj->vert_count;
            dataI += sizeof(uint32_t) * obj->indx_count;
        }

        child = child->next;
    }
       
    glBindVertexArrayOES(gui.go.graphObj.shapes[0].VAO);

    glBindBuffer(GL_ARRAY_BUFFER, gui.go.graphObj.shapes[0].vParam.buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2DGUI) * gui.go.graphObj.shapes[0].vParam.num_verts, gui.go.graphObj.shapes[0].vParam.vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui.go.graphObj.shapes[0].iParam.buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * gui.go.graphObj.shapes[0].iParam.indexesSize, gui.go.graphObj.shapes[0].iParam.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

    glGenVertexArraysOES(0, NULL); 
}

void GUIManagerObjDestroy(GUIObj *obj){
    free(obj->indeces);
    free(obj->points);
}

void GUIManagerClear(){
    ChildStack *child = gui.draw_list;
    
    if(child == NULL)
        return;

    ChildStack *next = NULL;
    ChildStack *before = NULL;
    
    uint32_t counter = 0;
       
    while(child != NULL){    
        
        if(child->node != NULL){
            GUIManagerObjDestroy(child->node);
            free(child->node);
        }

        before = child;  
        child = child->next;

        free(before);
        before = NULL;
    }
    
    gui.draw_list = NULL;
    gui.currIndx = 0;
    gui.sellected = false;
}

void GUIManagerInitFont(int default_font){
        
    gui.font.info = (stbtt_fontinfo *) AllocateMemory(1, sizeof(stbtt_fontinfo));
    gui.font.cdata = (stbtt_bakedchar *) AllocateMemory(1106, sizeof(stbtt_fontinfo));
    
    uint8_t *temp_bitmap = AllocateMemory( gui.font.fontWidth *  gui.font.fontHeight, sizeof(uint8_t));

    if(!default_font){
        FILE *font = fopen(engine.DataR.font_path, "rb");

        printf("Font name is : %s\n", engine.DataR.font_path);

        if(font){
            fseek(font, 0L, SEEK_END);
            uint32_t size = ftell(font);

            const unsigned char *buff = (const unsigned char *)AllocateMemoryP(size, sizeof(char), &engine);

            fseek(font, 0L, SEEK_SET);
            
            fread((void *)buff, sizeof(char),size, font);

            stbtt_InitFont(gui.font.info, buff, stbtt_GetFontOffsetForIndex(buff,0));
            stbtt_BakeFontBitmap(buff, 0, 32.0, temp_bitmap, gui.font.fontWidth, gui.font.fontHeight, 0, 1106, gui.font.cdata); // no guarantee this fits!

            FreeMemory((void *)buff);
            fclose(font);
        }
    }else{
        stbtt_BakeFontBitmap(RobotoBlack_ttf, 0, 32.0, temp_bitmap, gui.font.fontWidth, gui.font.fontHeight, 0, 1106, gui.font.cdata); // no guarantee this fits!
    }
    
    uint8_t *point = temp_bitmap;

    vec2 TexUvScale = vec2_f(1.0f / gui.font.fontWidth, 1.0f / gui.font.fontHeight);

    point[0] = point[1] = point[gui.font.fontWidth] = point[gui.font.fontWidth + 1] = 0xFF;

    TexUvWhitePixel = vec2_f(0.5f * TexUvScale.x, 0.5f * TexUvScale.y);

    uint32_t *temp_bitmap2 = AllocateMemory( gui.font.fontWidth *  gui.font.fontHeight, sizeof(uint32_t));       
    
    glGenTextures(1, &gui.font.buffer);
    glBindTexture(GL_TEXTURE_2D, gui.font.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, gui.font.fontWidth, gui.font.fontHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);

    free(temp_bitmap);
            
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GUIManagerInit(int default_font){
    
    for (int i = 0; i < GUI_ARRAYSIZE(ArcFastVtx); i++)
    {
        const float a = ((float)i * 2 * M_PI) / (float)GUI_ARRAYSIZE(ArcFastVtx);
        ArcFastVtx[i] = vec2_f(cos(a), sin(a));
    }
    
    for (int i = 0; i < GUI_ARRAYSIZE(CircleSegmentCounts); i++)
    {
        const float radius = (float)i;
        CircleSegmentCounts[i] = (uint8_t)((i > 0) ? GUI_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(radius, CircleSegmentMaxError) : GUI_DRAWLIST_ARCFAST_SAMPLE_MAX);
    }

    ArcFastRadiusCutoff = GUI_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(GUI_DRAWLIST_ARCFAST_SAMPLE_MAX, CircleSegmentMaxError);

    memset(&gui, 0, sizeof(GUIManager));
    
    GameObject2DInit((GameObject2D *)&gui, TIGOR_GAME_OBJECT_TYPE_2D); 

    gui.go.transform.scale.x = engine.width;
    gui.go.transform.scale.y = engine.height;

    memcpy(gui.go.name, "GUI", 3);

    gui.font.fontWidth = 512;
    gui.font.fontHeight = 512;
    gui.font.fontSize = 14;
       
    GUIManagerInitFont(default_font);

    ////---------------------------------------------------------

    gui.go.graphObj.shapes[0].vParam.vertices = AllocateMemory(MAX_VERTEX_SIZE, sizeof(Vertex2DGUI));
    gui.go.graphObj.shapes[0].iParam.indices = AllocateMemory(MAX_INDEX_SIZE, sizeof(uint32_t));
    
    glGenVertexArraysOES(1, &gui.go.graphObj.shapes[0].VAO);
    glGenBuffers(1, &gui.go.graphObj.shapes[0].vParam.buffer);
    glGenBuffers(1, &gui.go.graphObj.shapes[0].iParam.buffer);
    glBindVertexArrayOES(gui.go.graphObj.shapes[0].VAO);

    glBindBuffer(GL_ARRAY_BUFFER, gui.go.graphObj.shapes[0].vParam.buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2DGUI) * gui.go.graphObj.shapes[0].vParam.num_verts, gui.go.graphObj.shapes[0].vParam.vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui.go.graphObj.shapes[0].iParam.buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * gui.go.graphObj.shapes[0].iParam.indexesSize, gui.go.graphObj.shapes[0].iParam.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

    glGenVertexArraysOES(0, NULL); 

    GUICurrVertexMaxCount = MAX_VERTEX_SIZE;
    GUICurrIndexMaxCount = MAX_INDEX_SIZE;

    gui.go.graphObj.num_shapes ++;

    GameObject2DSetShader((GameObject2D *)&gui, guiVertexShaderSource, guiFragmentShaderSource);
        
    GameObject2DInitDraw((GameObject2D *)&gui);
}

void GUIManagerDrawPrimRect(vec2 a, vec2 c, vec4 color){

    if(gui.draw_list == NULL)
        gui.draw_list = calloc(1, sizeof(ChildStack));

    GUIObj *rect = GUIManagerAddObject();

    rect->indeces = calloc(6, sizeof(uint32_t));
    rect->points = calloc(4, sizeof(Vertex2DGUI));

    vec2 b = {c.x, a.y};
    vec2 d = {a.x, c.y};

    vec2 uv = TexUvWhitePixel;
    
    uint32_t idx = gui.currIndx;

    rect->indeces[0] = idx; rect->indeces[1] = idx+1; rect->indeces[2] = idx+2;
    rect->indeces[3] = idx; rect->indeces[4] = idx+2; rect->indeces[5] = idx+3;


    rect->points[0].position = a; rect->points[0].texCoord = uv; rect->points[0].color = color;
    rect->points[1].position = b; rect->points[1].texCoord = uv; rect->points[1].color = color;
    rect->points[2].position = c; rect->points[2].texCoord = uv; rect->points[2].color = color;
    rect->points[3].position = d; rect->points[3].texCoord = uv; rect->points[3].color = color;

    rect->indx_count = 6;
    rect->vert_count = 4;

    gui.currIndx += 4;
}

void GUIManagerDrawRect(vec2 a, vec2 c, vec4 color){

    double xpos, ypos;

    TEngineGetCursorPos(&xpos, &ypos);

    xpos *= 2;
    ypos *= 2;

    if(a.x < xpos && c.x > xpos && a.y < ypos && c.y > ypos && TEngineGetMousePress(TIGOR_MOUSE_BUTTON_1) && !gui.sellected){
        color = v4_subs(color, 0.1);
        gui.sellected =true;
    }

    GUIManagerDrawPrimRect(a, c, color);

}

void GUIAddRectFilled(const vec2 p_min, const vec2 p_max, vec4 col, float rounding, uint32_t flags)
{
    if (rounding < 0.5f)
    {
        GUIManagerDrawPrimRect(p_min, p_max, col);
    }
    else
    {
        PathRect(p_min, p_max, rounding, flags);
        PathFillConvex(col);
    }
}

void GUISetText(float xpos, float ypos, vec4 color, float font_size, uint32_t *text){

    int len = ToolsStr32BitLength((uint32_t *)text);

    stbtt_aligned_quad q;
    
    float x = 0.0f;
    float y = 0.0f;
    
    if(gui.draw_list == NULL)
        gui.draw_list = calloc(1, sizeof(ChildStack));
        
    GUIObj *rect = GUIManagerAddObject();

    uint32_t *tempI = text;

    rect->points = calloc(len * 4, sizeof(Vertex2DGUI));
    rect->indeces = calloc(len * 6, sizeof(uint32_t));
    
    float mulX = font_size / GUIFontResizer;
    float mulY = font_size / GUIFontResizer;
    
    float temp = font_size;

    uint32_t v_iter = 0;
    uint32_t i_iter = 0;
    // Generate a uv mapped quad per char in the new text
    for (int i=0;i < len;i++)
    {
        stbtt_GetBakedQuad(gui.font.cdata, 512,512, *tempI, &x,&y,&q,1);//1=opengl & d3d10+,0=d3d9

        
        float x1 = xpos + q.x0 * mulX - 1.0f;
        float x2 = xpos + q.x1 * mulX - 1.0f;
        float y1 = ypos + q.y0 * mulY - 1.0f + temp;
        float y2 = ypos + q.y1 * mulY - 1.0f + temp;
        float u1 = q.s0;
        float v1 = q.t0;
        float u2 = q.s1;
        float v2 = q.t1;

        rect->points[v_iter + 0].position.x = x1; rect->points[v_iter + 0].position.y = y1; rect->points[v_iter + 0].color = color; rect->points[v_iter + 0].texCoord.x = u1; rect->points[v_iter + 0].texCoord.y = v1;
        rect->points[v_iter + 1].position.x = x2; rect->points[v_iter + 1].position.y = y1; rect->points[v_iter + 1].color = color; rect->points[v_iter + 1].texCoord.x = u2; rect->points[v_iter + 1].texCoord.y = v1;
        rect->points[v_iter + 2].position.x = x2; rect->points[v_iter + 2].position.y = y2; rect->points[v_iter + 2].color = color; rect->points[v_iter + 2].texCoord.x = u2; rect->points[v_iter + 2].texCoord.y = v2;
        rect->points[v_iter + 3].position.x = x1; rect->points[v_iter + 3].position.y = y2; rect->points[v_iter + 3].color = color; rect->points[v_iter + 3].texCoord.x = u1; rect->points[v_iter + 3].texCoord.y = v2;
        
        rect->indeces[i_iter + 0] = gui.currIndx; rect->indeces[i_iter + 1] = gui.currIndx + 1; rect->indeces[i_iter + 2] =  gui.currIndx + 2;
        rect->indeces[i_iter + 3] = gui.currIndx; rect->indeces[i_iter + 4] = gui.currIndx + 2; rect->indeces[i_iter + 5] =  gui.currIndx + 3;

        v_iter += 4;
        i_iter += 6;
        gui.currIndx += 4;
        ++tempI;
    }

    rect->indx_count = i_iter;
    rect->vert_count = v_iter;
}

void GUIAddTextU8(float xpos, float ypos, vec4 color, float font_size, char *text){

    if(!GUIManagerIsInit())
        return;

    uint32_t size = strlen(text) + 1;
    uint32_t buff[size + 1];
    memset(buff, 0, size + 1);

    //ToolsStringToUInt32(buff, text);

    ToolsTextStrFromUtf8(buff, size, text, 0, NULL);

    GUIAddTextU32(xpos, ypos, color, font_size, buff);
}

void GUIAddTextU32(float xpos, float ypos, vec4 color, float font_size, uint32_t *text){
    
    if(!GUIManagerIsInit())
        return;

    GUISetText(xpos, ypos, color, font_size, text);
}

void AddConvexPolyFilled(const vec2 *points, const int points_count, vec4 col)
{

    if (points_count < 3)
        return;

    const vec2 uv = TexUvWhitePixel;
    
    if(gui.draw_list == NULL)
        gui.draw_list = calloc(1, sizeof(ChildStack));
        
    GUIObj *rect = GUIManagerAddObject();

    uint32_t v_iter = 0;
    uint32_t i_iter = 0;

    /*if (Flags & ImDrawListFlags_AntiAliasedFill)
    {
        // Anti-aliased Fill
        const float AA_SIZE = _FringeScale;
        const ImU32 col_trans = col & ~IM_COL32_A_MASK;
        const int idx_count = (points_count - 2)*3 + points_count * 6;
        const int vtx_count = (points_count * 2);
        PrimReserve(idx_count, vtx_count);

        // Add indexes for fill
        unsigned int vtx_inner_idx = _VtxCurrentIdx;
        unsigned int vtx_outer_idx = _VtxCurrentIdx + 1;
        for (int i = 2; i < points_count; i++)
        {
            _IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx); _IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx + ((i - 1) << 1)); _IdxWritePtr[2] = (ImDrawIdx)(vtx_inner_idx + (i << 1));
            _IdxWritePtr += 3;
        }

        // Compute normals
        _Data->TempBuffer.reserve_discard(points_count);
        ImVec2* temp_normals = _Data->TempBuffer.Data;
        for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            const ImVec2& p0 = points[i0];
            const ImVec2& p1 = points[i1];
            float dx = p1.x - p0.x;
            float dy = p1.y - p0.y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i0].x = dy;
            temp_normals[i0].y = -dx;
        }

        for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            // Average normals
            const ImVec2& n0 = temp_normals[i0];
            const ImVec2& n1 = temp_normals[i1];
            float dm_x = (n0.x + n1.x) * 0.5f;
            float dm_y = (n0.y + n1.y) * 0.5f;
            IM_FIXNORMAL2F(dm_x, dm_y);
            dm_x *= AA_SIZE * 0.5f;
            dm_y *= AA_SIZE * 0.5f;

            // Add vertices
            _VtxWritePtr[0].pos.x = (points[i1].x - dm_x); _VtxWritePtr[0].pos.y = (points[i1].y - dm_y); _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;        // Inner
            _VtxWritePtr[1].pos.x = (points[i1].x + dm_x); _VtxWritePtr[1].pos.y = (points[i1].y + dm_y); _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col_trans;  // Outer
            _VtxWritePtr += 2;

            // Add indexes for fringes
            _IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx + (i1 << 1)); _IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx + (i0 << 1)); _IdxWritePtr[2] = (ImDrawIdx)(vtx_outer_idx + (i0 << 1));
            _IdxWritePtr[3] = (ImDrawIdx)(vtx_outer_idx + (i0 << 1)); _IdxWritePtr[4] = (ImDrawIdx)(vtx_outer_idx + (i1 << 1)); _IdxWritePtr[5] = (ImDrawIdx)(vtx_inner_idx + (i1 << 1));
            _IdxWritePtr += 6;
        }
        _VtxCurrentIdx += (ImDrawIdx)vtx_count;
    }
    else*/
    {
        // Non Anti-aliased Fill
        const int idx_count = (points_count - 2)*3;
        const int vtx_count = points_count;
        
        
        rect->indeces = calloc(idx_count, sizeof(uint32_t));
        rect->points = calloc(vtx_count, sizeof(Vertex2DGUI));

        for (int i = 0; i < vtx_count; i++)
        {
            rect->points[v_iter].position = v2_subs(points[i], 1.0f); rect->points[v_iter].texCoord = uv; rect->points[v_iter].color = col;
            v_iter ++;
        }
        for (int i = 2; i < points_count; i++)
        {
            rect->indeces[i_iter + 0] = gui.currIndx; rect->indeces[i_iter + 1] = gui.currIndx + i - 1; rect->indeces[i_iter + 2] = gui.currIndx + i;
            i_iter +=3;
        }
        
        rect->indx_count = idx_count;
        rect->vert_count = vtx_count;

        gui.currIndx += vtx_count;
    }
}

void GUIManagerAddPolyline(const vec2* points, int points_count, vec4 color, DrawListFlags flags, float thickness){

    if (points_count < 2)
        return;

    thickness /=10;
    
    const bool closed = (flags & GUIDrawFlags_Closed) != 0;
    const int count = closed ? points_count : points_count - 1; // The number of line segments we need to draw
    const vec2 opaque_uv = TexUvWhitePixel;
    const bool thick_line = (thickness > gui._FringeScale);

    
    if(gui.draw_list == NULL)
        gui.draw_list = calloc(1, sizeof(ChildStack));
        
    GUIObj *rect = GUIManagerAddObject();

    rect->indeces = calloc(count * 6, sizeof(uint32_t));
    rect->points = calloc(count * 4, sizeof(Vertex2DGUI));

    uint32_t v_iter = 0;
    uint32_t i_iter = 0;
    /*if (gui.Flags & GUIDrawListFlags_AntiAliasedLines)
    {
        // Anti-aliased stroke
        const float AA_SIZE = gui._FringeScale;

        // Thicknesses <1.0 should behave like thickness 1.0
        thickness = ImMax(thickness, 1.0f);
        const int integer_thickness = (int)thickness;
        const float fractional_thickness = thickness - integer_thickness;

        // Do we want to draw this line using a texture?
        // - For now, only draw integer-width lines using textures to avoid issues with the way scaling occurs, could be improved.
        // - If AA_SIZE is not 1.0f we cannot use the texture path.
        const bool use_texture = (gui.Flags & GUIDrawListFlags_AntiAliasedLinesUseTex) && (integer_thickness < IM_DRAWLIST_TEX_LINES_WIDTH_MAX) && (fractional_thickness <= 0.00001f) && (AA_SIZE == 1.0f);

        // We should never hit this, because NewFrame() doesn't set ImDrawListFlags_AntiAliasedLinesUseTex unless ImFontAtlasFlags_NoBakedLines is off
        //IM_ASSERT_PARANOID(!use_texture || !(gui._Data->Font->ContainerAtlas->Flags & GUIFontAtlasFlags_NoBakedLines));

        const int idx_count = use_texture ? (count * 6) : (thick_line ? count * 18 : count * 12);
        const int vtx_count = use_texture ? (points_count * 2) : (thick_line ? points_count * 4 : points_count * 3);
        PrimReserve(idx_count, vtx_count);

        // Temporary buffer
        // The first <points_count> items are normals at each line point, then after that there are either 2 or 4 temp points for each line point
        _Data->TempBuffer.reserve_discard(points_count * ((use_texture || !thick_line) ? 3 : 5));
        vec2* temp_normals = _Data->TempBuffer.Data;
        vec2* temp_points = temp_normals + points_count;

        // Calculate normals (tangents) for each line segment
        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
            float dx = points[i2].x - points[i1].x;
            float dy = points[i2].y - points[i1].y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i1].x = dy;
            temp_normals[i1].y = -dx;
        }
        if (!closed)
            temp_normals[points_count - 1] = temp_normals[points_count - 2];

        // If we are drawing a one-pixel-wide line without a texture, or a textured line of any width, we only need 2 or 3 vertices per point
        if (use_texture || !thick_line)
        {
            // [PATH 1] Texture-based lines (thick or non-thick)
            // [PATH 2] Non texture-based lines (non-thick)

            // The width of the geometry we need to draw - this is essentially <thickness> pixels for the line itself, plus "one pixel" for AA.
            // - In the texture-based path, we don't use AA_SIZE here because the +1 is tied to the generated texture
            //   (see ImFontAtlasBuildRenderLinesTexData() function), and so alternate values won't work without changes to that code.
            // - In the non texture-based paths, we would allow AA_SIZE to potentially be != 1.0f with a patch (e.g. fringe_scale patch to
            //   allow scaling geometry while preserving one-screen-pixel AA fringe).
            const float half_draw_size = use_texture ? ((thickness * 0.5f) + 1) : AA_SIZE;

            // If line is not closed, the first and last points need to be generated differently as there are no normals to blend
            if (!closed)
            {
                temp_points[0] = points[0] + temp_normals[0] * half_draw_size;
                temp_points[1] = points[0] - temp_normals[0] * half_draw_size;
                temp_points[(points_count-1)*2+0] = points[points_count-1] + temp_normals[points_count-1] * half_draw_size;
                temp_points[(points_count-1)*2+1] = points[points_count-1] - temp_normals[points_count-1] * half_draw_size;
            }

            // Generate the indices to form a number of triangles for each line segment, and the vertices for the line edges
            // This takes points n and n+1 and writes into n+1, with the first point in a closed line being generated from the final one (as n+1 wraps)
            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            unsigned int idx1 = gui.currIndx; // Vertex index for start of line segment
            for (int i1 = 0; i1 < count; i1++) // i1 is the first point of the line segment
            {
                const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1; // i2 is the second point of the line segment
                const unsigned int idx2 = ((i1 + 1) == points_count) ? _VtxCurrentIdx : (idx1 + (use_texture ? 2 : 3)); // Vertex index for end of segment

                // Average normals
                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                IM_FIXNORMAL2F(dm_x, dm_y);
                dm_x *= half_draw_size; // dm_x, dm_y are offset to the outer edge of the AA area
                dm_y *= half_draw_size;

                // Add temporary vertexes for the outer edges
                vec2* out_vtx = &temp_points[i2 * 2];
                out_vtx[0].x = points[i2].x + dm_x;
                out_vtx[0].y = points[i2].y + dm_y;
                out_vtx[1].x = points[i2].x - dm_x;
                out_vtx[1].y = points[i2].y - dm_y;

                if (use_texture)
                {
                    // Add indices for two triangles
                    _IdxWritePtr[0] = (ImDrawIdx)(idx2 + 0); _IdxWritePtr[1] = (ImDrawIdx)(idx1 + 0); _IdxWritePtr[2] = (ImDrawIdx)(idx1 + 1); // Right tri
                    _IdxWritePtr[3] = (ImDrawIdx)(idx2 + 1); _IdxWritePtr[4] = (ImDrawIdx)(idx1 + 1); _IdxWritePtr[5] = (ImDrawIdx)(idx2 + 0); // Left tri
                    _IdxWritePtr += 6;
                }
                else
                {
                    // Add indexes for four triangles
                    _IdxWritePtr[0] = (ImDrawIdx)(idx2 + 0); _IdxWritePtr[1] = (ImDrawIdx)(idx1 + 0); _IdxWritePtr[2] = (ImDrawIdx)(idx1 + 2); // Right tri 1
                    _IdxWritePtr[3] = (ImDrawIdx)(idx1 + 2); _IdxWritePtr[4] = (ImDrawIdx)(idx2 + 2); _IdxWritePtr[5] = (ImDrawIdx)(idx2 + 0); // Right tri 2
                    _IdxWritePtr[6] = (ImDrawIdx)(idx2 + 1); _IdxWritePtr[7] = (ImDrawIdx)(idx1 + 1); _IdxWritePtr[8] = (ImDrawIdx)(idx1 + 0); // Left tri 1
                    _IdxWritePtr[9] = (ImDrawIdx)(idx1 + 0); _IdxWritePtr[10] = (ImDrawIdx)(idx2 + 0); _IdxWritePtr[11] = (ImDrawIdx)(idx2 + 1); // Left tri 2
                    _IdxWritePtr += 12;
                }

                idx1 = idx2;
            }

            // Add vertexes for each point on the line
            if (use_texture)
            {
                // If we're using textures we only need to emit the left/right edge vertices
                ImVec4 tex_uvs = _Data->TexUvLines[integer_thickness];
                if (fractional_thickness != 0.0f) // Currently always zero when use_texture==false!
                {
                    const ImVec4 tex_uvs_1 = _Data->TexUvLines[integer_thickness + 1];
                    tex_uvs.x = tex_uvs.x + (tex_uvs_1.x - tex_uvs.x) * fractional_thickness; // inlined ImLerp()
                    tex_uvs.y = tex_uvs.y + (tex_uvs_1.y - tex_uvs.y) * fractional_thickness;
                    tex_uvs.z = tex_uvs.z + (tex_uvs_1.z - tex_uvs.z) * fractional_thickness;
                    tex_uvs.w = tex_uvs.w + (tex_uvs_1.w - tex_uvs.w) * fractional_thickness;
                }
                ImVec2 tex_uv0(tex_uvs.x, tex_uvs.y);
                ImVec2 tex_uv1(tex_uvs.z, tex_uvs.w);
                for (int i = 0; i < points_count; i++)
                {
                    _VtxWritePtr[0].pos = temp_points[i * 2 + 0]; _VtxWritePtr[0].uv = tex_uv0; _VtxWritePtr[0].col = col; // Left-side outer edge
                    _VtxWritePtr[1].pos = temp_points[i * 2 + 1]; _VtxWritePtr[1].uv = tex_uv1; _VtxWritePtr[1].col = col; // Right-side outer edge
                    _VtxWritePtr += 2;
                }
            }
            else
            {
                // If we're not using a texture, we need the center vertex as well
                for (int i = 0; i < points_count; i++)
                {
                    _VtxWritePtr[0].pos = points[i];              _VtxWritePtr[0].uv = opaque_uv; _VtxWritePtr[0].col = col;       // Center of line
                    _VtxWritePtr[1].pos = temp_points[i * 2 + 0]; _VtxWritePtr[1].uv = opaque_uv; _VtxWritePtr[1].col = col_trans; // Left-side outer edge
                    _VtxWritePtr[2].pos = temp_points[i * 2 + 1]; _VtxWritePtr[2].uv = opaque_uv; _VtxWritePtr[2].col = col_trans; // Right-side outer edge
                    _VtxWritePtr += 3;
                }
            }
        }
        else
        {
            // [PATH 2] Non texture-based lines (thick): we need to draw the solid line core and thus require four vertices per point
            const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;

            // If line is not closed, the first and last points need to be generated differently as there are no normals to blend
            if (!closed)
            {
                const int points_last = points_count - 1;
                temp_points[0] = points[0] + temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[1] = points[0] + temp_normals[0] * (half_inner_thickness);
                temp_points[2] = points[0] - temp_normals[0] * (half_inner_thickness);
                temp_points[3] = points[0] - temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[points_last * 4 + 0] = points[points_last] + temp_normals[points_last] * (half_inner_thickness + AA_SIZE);
                temp_points[points_last * 4 + 1] = points[points_last] + temp_normals[points_last] * (half_inner_thickness);
                temp_points[points_last * 4 + 2] = points[points_last] - temp_normals[points_last] * (half_inner_thickness);
                temp_points[points_last * 4 + 3] = points[points_last] - temp_normals[points_last] * (half_inner_thickness + AA_SIZE);
            }

            // Generate the indices to form a number of triangles for each line segment, and the vertices for the line edges
            // This takes points n and n+1 and writes into n+1, with the first point in a closed line being generated from the final one (as n+1 wraps)
            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            unsigned int idx1 = _VtxCurrentIdx; // Vertex index for start of line segment
            for (int i1 = 0; i1 < count; i1++) // i1 is the first point of the line segment
            {
                const int i2 = (i1 + 1) == points_count ? 0 : (i1 + 1); // i2 is the second point of the line segment
                const unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : (idx1 + 4); // Vertex index for end of segment

                // Average normals
                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                IM_FIXNORMAL2F(dm_x, dm_y);
                float dm_out_x = dm_x * (half_inner_thickness + AA_SIZE);
                float dm_out_y = dm_y * (half_inner_thickness + AA_SIZE);
                float dm_in_x = dm_x * half_inner_thickness;
                float dm_in_y = dm_y * half_inner_thickness;

                // Add temporary vertices
                ImVec2* out_vtx = &temp_points[i2 * 4];
                out_vtx[0].x = points[i2].x + dm_out_x;
                out_vtx[0].y = points[i2].y + dm_out_y;
                out_vtx[1].x = points[i2].x + dm_in_x;
                out_vtx[1].y = points[i2].y + dm_in_y;
                out_vtx[2].x = points[i2].x - dm_in_x;
                out_vtx[2].y = points[i2].y - dm_in_y;
                out_vtx[3].x = points[i2].x - dm_out_x;
                out_vtx[3].y = points[i2].y - dm_out_y;

                // Add indexes
                _IdxWritePtr[0]  = (ImDrawIdx)(idx2 + 1); _IdxWritePtr[1]  = (ImDrawIdx)(idx1 + 1); _IdxWritePtr[2]  = (ImDrawIdx)(idx1 + 2);
                _IdxWritePtr[3]  = (ImDrawIdx)(idx1 + 2); _IdxWritePtr[4]  = (ImDrawIdx)(idx2 + 2); _IdxWritePtr[5]  = (ImDrawIdx)(idx2 + 1);
                _IdxWritePtr[6]  = (ImDrawIdx)(idx2 + 1); _IdxWritePtr[7]  = (ImDrawIdx)(idx1 + 1); _IdxWritePtr[8]  = (ImDrawIdx)(idx1 + 0);
                _IdxWritePtr[9]  = (ImDrawIdx)(idx1 + 0); _IdxWritePtr[10] = (ImDrawIdx)(idx2 + 0); _IdxWritePtr[11] = (ImDrawIdx)(idx2 + 1);
                _IdxWritePtr[12] = (ImDrawIdx)(idx2 + 2); _IdxWritePtr[13] = (ImDrawIdx)(idx1 + 2); _IdxWritePtr[14] = (ImDrawIdx)(idx1 + 3);
                _IdxWritePtr[15] = (ImDrawIdx)(idx1 + 3); _IdxWritePtr[16] = (ImDrawIdx)(idx2 + 3); _IdxWritePtr[17] = (ImDrawIdx)(idx2 + 2);
                _IdxWritePtr += 18;

                idx1 = idx2;
            }

            // Add vertices
            for (int i = 0; i < points_count; i++)
            {
                _VtxWritePtr[0].pos = temp_points[i * 4 + 0]; _VtxWritePtr[0].uv = opaque_uv; _VtxWritePtr[0].col = col_trans;
                _VtxWritePtr[1].pos = temp_points[i * 4 + 1]; _VtxWritePtr[1].uv = opaque_uv; _VtxWritePtr[1].col = col;
                _VtxWritePtr[2].pos = temp_points[i * 4 + 2]; _VtxWritePtr[2].uv = opaque_uv; _VtxWritePtr[2].col = col;
                _VtxWritePtr[3].pos = temp_points[i * 4 + 3]; _VtxWritePtr[3].uv = opaque_uv; _VtxWritePtr[3].col = col_trans;
                _VtxWritePtr += 4;
            }
        }
        _VtxCurrentIdx += (ImDrawIdx)vtx_count;
    }
    else*/
    {
        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
            const vec2 p1 = v2_subs(points[i1], 1.0f);
            const vec2 p2 = v2_subs(points[i2], 1.0f);

            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            GUI_NORMALIZE2F_OVER_ZERO(dx, dy);
            dx *= (thickness * 0.5f);
            dy *= (thickness * 0.5f);

            rect->points[v_iter + 0].position.x = p1.x + dy; rect->points[v_iter + 0].position.y = p1.y - dx; rect->points[v_iter + 0].texCoord = opaque_uv; rect->points[v_iter + 0].color = color;
            rect->points[v_iter + 1].position.x = p2.x + dy; rect->points[v_iter + 1].position.y = p2.y - dx; rect->points[v_iter + 1].texCoord = opaque_uv; rect->points[v_iter + 1].color = color;
            rect->points[v_iter + 2].position.x = p2.x - dy; rect->points[v_iter + 2].position.y = p2.y + dx; rect->points[v_iter + 2].texCoord = opaque_uv; rect->points[v_iter + 2].color = color;
            rect->points[v_iter + 3].position.x = p1.x - dy; rect->points[v_iter + 3].position.y = p1.y + dx; rect->points[v_iter + 3].texCoord = opaque_uv; rect->points[v_iter + 3].color = color;
            v_iter += 4;

            rect->indeces[i_iter + 0] = gui.currIndx; rect->indeces[i_iter + 1] = gui.currIndx + 1; rect->indeces[i_iter + 2] = gui.currIndx + 2;
            rect->indeces[i_iter + 3] = gui.currIndx; rect->indeces[i_iter + 4] = gui.currIndx + 2; rect->indeces[i_iter + 5] = gui.currIndx + 3;
            i_iter += 6;
            

            gui.currIndx += 4;
        }
        
        rect->indx_count = i_iter;
        rect->vert_count = v_iter;
    }
}

DrawFlags FixRectCornerFlags(DrawFlags flags)
{
    /*
    IM_STATIC_ASSERT(ImDrawFlags_RoundCornersTopLeft == (1 << 4));
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    // Obsoleted in 1.82 (from February 2021). This code was stripped/simplified and mostly commented in 1.90 (from September 2023)
    // - Legacy Support for hard coded ~0 (used to be a suggested equivalent to ImDrawCornerFlags_All)
    if (flags == ~0)                    { return ImDrawFlags_RoundCornersAll; }
    // - Legacy Support for hard coded 0x01 to 0x0F (matching 15 out of 16 old flags combinations). Read details in older version of this code.
    if (flags >= 0x01 && flags <= 0x0F) { return (flags << 4); }
    // We cannot support hard coded 0x00 with 'float rounding > 0.0f' --> replace with ImDrawFlags_RoundCornersNone or use 'float rounding = 0.0f'
#endif
    */
    // If this assert triggers, please update your code replacing hardcoded values with new ImDrawFlags_RoundCorners* values.
    // Note that ImDrawFlags_Closed (== 0x01) is an invalid flag for AddRect(), AddRectFilled(), PathRect() etc. anyway.
    // See details in 1.82 Changelog as well as 2021/03/12 and 2023/09/08 entries in "API BREAKING CHANGES" section.
    GUI_ASSERT((flags & 0x0F) == 0 && "Misuse of legacy hardcoded ImDrawCornerFlags values!");

    if ((flags & GUIDrawFlags_RoundCornersMask_) == 0)
        flags |= GUIDrawFlags_RoundCornersAll;

    return flags;
}

void PathRect(vec2 a, vec2 b, float rounding, uint32_t flags){

    if (rounding >= 0.5f)
    {
        flags = FixRectCornerFlags(flags);
        rounding = e_min(rounding, fabs(b.x - a.x) * (((flags & GUIDrawFlags_RoundCornersTop) == GUIDrawFlags_RoundCornersTop) || ((flags & GUIDrawFlags_RoundCornersBottom) == GUIDrawFlags_RoundCornersBottom) ? 0.5f : 1.0f) - 1.0f);
        rounding = e_min(rounding, fabs(b.y - a.y) * (((flags & GUIDrawFlags_RoundCornersLeft) == GUIDrawFlags_RoundCornersLeft) || ((flags & GUIDrawFlags_RoundCornersRight) == GUIDrawFlags_RoundCornersRight) ? 0.5f : 1.0f) - 1.0f);
    }

    if (rounding < 0.5f || (flags & GUIDrawFlags_RoundCornersMask_) == GUIDrawFlags_RoundCornersNone)
    {
        PathLineTo(a);
        PathLineTo(vec2_f(b.x, a.y));
        PathLineTo(b);
        PathLineTo(vec2_f(a.x, b.y));
    }
    else
    {      
        const float rounding_tl = (flags & GUIDrawFlags_RoundCornersTopLeft)     ? rounding : 0.0f;
        const float rounding_tr = (flags & GUIDrawFlags_RoundCornersTopRight)    ? rounding : 0.0f;
        const float rounding_br = (flags & GUIDrawFlags_RoundCornersBottomRight) ? rounding : 0.0f;
        const float rounding_bl = (flags & GUIDrawFlags_RoundCornersBottomLeft)  ? rounding : 0.0f;
        PathArcToFast(vec2_f(a.x + rounding_tl, a.y + rounding_tl), rounding_tl, 6, 9);
        PathArcToFast(vec2_f(b.x - rounding_tr, a.y + rounding_tr), rounding_tr, 9, 12);
        PathArcToFast(vec2_f(b.x - rounding_br, b.y - rounding_br), rounding_br, 0, 3);
        PathArcToFast(vec2_f(a.x + rounding_bl, b.y - rounding_bl), rounding_bl, 3, 6);
    }
}

void PathLineTo(vec2 pos)  { 
        
    gui._Path[gui._Path_Size] = pos; 
    
    gui._Path_Size ++; 

}

void PathFillConvex(vec4 col){ 
    
    if(!GUIManagerIsInit()){
        memset(gui._Path, 0, sizeof(vec2) * 32);
        gui._Path_Size = 0;
        return;
    }

    AddConvexPolyFilled(gui._Path, gui._Path_Size, col); 
    
    memset(gui._Path, 0, sizeof(vec2) * 32);
    gui._Path_Size = 0; 
}

void PathStroke(vec4 color, uint32_t flags, float thickness) { 
    
    if(!GUIManagerIsInit()){
        memset(gui._Path, 0, sizeof(vec2) * 32);
        gui._Path_Size = 0;
        return;
    }

    GUIManagerAddPolyline(gui._Path, gui._Path_Size, color, flags, thickness); 
    
    memset(gui._Path, 0, sizeof(vec2) * 32);
    gui._Path_Size = 0; 
}

int GUIManagerIsInit(){
    return gui.go.self.flags & TIGOR_GAME_OBJECT_FLAG_INIT;
}

void GUIManagerUpdate(){    
    Camera2D* cam = (Camera2D*) engine.cam2D;    
        
    glUseProgram(gui.go.self.shaderProgram);


    mat4 model = m4_transform2D(gui.go.transform.position, gui.go.transform.scale, gui.go.transform.rotation);
    unsigned int tMat = glGetUniformLocation(gui.go.self.shaderProgram, "u_model");
    glUniformMatrix4fv(tMat, 1, GL_FALSE, (const float*) &model);
    
    mat4 view = m4_transform2D(v2_muls(cam->position, -1), cam->scale, cam->angle);
    tMat = glGetUniformLocation(gui.go.self.shaderProgram, "u_view");
    glUniformMatrix4fv(tMat, 1, GL_FALSE, (const float*) &view);
    
    mat4 proj = m4_ortho(0, engine.width, engine.height, 0.0, -1.0, 1.0f);
    tMat = glGetUniformLocation(gui.go.self.shaderProgram, "u_proj");
    glUniformMatrix4fv(tMat, 1, GL_FALSE, (const float*) &proj);
            
    
    glUseProgram(0);
}

void GUIManagerDraw(){
    
    if(GUIManagerObjCount() == 0)
        return;
    
    glUseProgram(gui.go.self.shaderProgram);
    
    uint32_t vCount, iCount;

    GUIManagerGetVertexCount(&vCount, &iCount);

    GUIManagerCopyVertex(vCount, iCount);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gui.font.buffer);

    for(int i=0; i < gui.go.graphObj.num_shapes;i++){   
        glBindVertexArrayOES(gui.go.graphObj.shapes[i].VAO); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui.go.graphObj.shapes[i].iParam.buffer);         
        glDrawElements(GL_TRIANGLES, gui.go.graphObj.shapes[i].iParam.indexesSize, GL_UNSIGNED_INT, 0);

    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void GUIManagerRecreate(){

    GameObjectClean((GameObject *)&gui.go);

    GameObjectRecreate((GameObject *)&gui.go);
}

void GUIManagerDestroy(){
    GUIManagerClear();   
    
    FreeMemory((void *)gui.font.cdata);
    FreeMemory((void *)gui.font.info);
    FreeMemory((void *)gui.font.texture);

    GameObject2DDestroy((GameObject2D *)&gui); 
    
    if(gui.draw_list != NULL){
        free(gui.draw_list);
        gui.draw_list = NULL;
    }
}
#ifndef MANAGER_INCLUDES_H
#define MANAGER_INCLUDES_H

typedef void* EGLConfig;
typedef void* EGLContext;
typedef void* EGLDisplay;
typedef void* EGLSurface;

typedef void* OSMesaContext;
typedef void (*OSMESAproc)(void);

typedef void (APIENTRY * PFNGLCLEARPROC)(uint32_t);
typedef const uint8_t* (APIENTRY * PFNGLGETSTRINGPROC)(uint32_t);
typedef void (APIENTRY * PFNGLGETINTEGERVPROC)(uint32_t,int32_t*);
typedef const uint8_t* (APIENTRY * PFNGLGETSTRINGIPROC)(uint32_t,uint32_t);

typedef struct wManagerWindow wManagerWindow;

typedef struct wManagerImage
{
    /*! The width, in pixels, of this image.
     */
    int width;
    /*! The height, in pixels, of this image.
     */
    int height;
    /*! The pixel data of this image, arranged left-to-right, top-to-bottom.
     */
    unsigned char* pixels;
} wManagerImage;

// Context structure
//
typedef struct _wManagercontext
{
    int                 client;
    int                 source;
    int                 major, minor, revision;
    int32_t            forward, debug, noerror;
    int                 profile;
    int                 robustness;
    int                 release;

    PFNGLGETSTRINGIPROC  GetStringi;
    PFNGLGETINTEGERVPROC GetIntegerv;
    PFNGLGETSTRINGPROC   GetString;

    void (*makeCurrent)(wManagerWindow*);
    void (*swapBuffers)(wManagerWindow*);
    void (*swapInterval)(int);
    int (*extensionSupported)(const char*);
    void* (*getProcAddress)(const char*);
    void (*destroy)(wManagerWindow*);

    struct {
        EGLConfig       config;
        EGLContext      handle;
        EGLSurface      surface;
        void*           client;
    } egl;

    struct {
        OSMesaContext   handle;
        int             width;
        int             height;
        void*           buffer;
    } osmesa;

} _wManagercontext;

// Initialization configuration
//
// Parameters relating to the initialization of the library
//
typedef struct _wManagerinitconfig
{
    int32_t      hatButtons;
    int           angleType;
    int           platformID;
    //PFN_vkGetInstanceProcAddr vulkanLoader;
    struct {
        int32_t  menubar;
        int32_t  chdir;
    } ns;
    struct {
        int32_t  xcbVulkanSurface;
    } x11;
    struct {
        int       libdecorMode;
    } wl;
} _wManagerinitconfig;

// Window configuration
//
// Parameters relating to the creation of the window but not directly related
// to the framebuffer.  This is used to pass window creation parameters from
// shared code to the platform API.
//
typedef struct _wManagerwndconfig
{
    int           xpos;
    int           ypos;
    int           width;
    int           height;
    const char*   title;
    int32_t      resizable;
    int32_t      visible;
    int32_t      decorated;
    int32_t      focused;
    int32_t      autoIconify;
    int32_t      floating;
    int32_t      maximized;
    int32_t      centerCursor;
    int32_t      focusOnShow;
    int32_t      mousePassthrough;
    int32_t      scaleToMonitor;
    struct {
        int32_t  retina;
        char      frameName[256];
    } ns;
    struct {
        char      className[256];
        char      instanceName[256];
    } x11;
    struct {
        int32_t  keymenu;
        int32_t  showDefault;
    } win32;
    struct {
        char      appId[256];
    } wl;
} _wManagerwndconfig;

// Framebuffer configuration
//
// This describes buffers and their sizes.  It also contains
// a platform-specific ID used to map back to the backend API object.
//
// It is used to pass framebuffer parameters from shared code to the platform
// API and also to enumerate and select available framebuffer configs.
//
typedef struct _wManagerfbconfig
{
    int         redBits;
    int         greenBits;
    int         blueBits;
    int         alphaBits;
    int         depthBits;
    int         stencilBits;
    int         accumRedBits;
    int         accumGreenBits;
    int         accumBlueBits;
    int         accumAlphaBits;
    int         auxBuffers;
    int32_t    stereo;
    int         samples;
    int32_t    sRGB;
    int32_t    doublebuffer;
    int32_t    transparent;
    uintptr_t   handle;
} _wManagerfbconfig;

typedef struct wManagerVidmode
{
    /*! The width, in screen coordinates, of the video mode.
     */
    int width;
    /*! The height, in screen coordinates, of the video mode.
     */
    int height;
    /*! The bit depth of the red channel of the video mode.
     */
    int redBits;
    /*! The bit depth of the green channel of the video mode.
     */
    int greenBits;
    /*! The bit depth of the blue channel of the video mode.
     */
    int blueBits;
    /*! The refresh rate, in Hz, of the video mode.
     */
    int refreshRate;
} wManagerVidmode;

typedef struct wManagerGammaramp
{
    /*! An array of value describing the response of the red channel.
     */
    unsigned short* red;
    /*! An array of value describing the response of the green channel.
     */
    unsigned short* green;
    /*! An array of value describing the response of the blue channel.
     */
    unsigned short* blue;
    /*! The number of elements in each array.
     */
    unsigned int size;
} wManagerGammaramp;

// Monitor structure
//
typedef struct _wManagerMonitor
{
    char            name[128];
    void*           userPointer;

    // Physical dimensions in millimeters.
    int             widthMM, heightMM;

    // The window whose video mode is current on this monitor
    wManagerWindow*    window;

    wManagerVidmode*    modes;
    int             modeCount;
    wManagerVidmode     currentMode;

    wManagerGammaramp   originalRamp;
    wManagerGammaramp   currentRamp;

    void *MonitorData;
} _wManagerMonitor;


// Platform API structure
//
typedef struct _wManagerPlatform
{
    int platformID;
    // init
    uint32_t (*init)(void);
    void (*terminate)(void);
    // input
    void (*getCursorPos)(wManagerWindow*,double*,double*);
    void (*setCursorPos)(wManagerWindow*,double,double);
    void (*setCursorMode)(wManagerWindow*,int);
    void (*setRawMouseMotion)(wManagerWindow*,uint32_t);
    uint32_t (*rawMouseMotionSupported)(void);
    //uint32_t (*createCursor)(_wManagercursor*,const wManagerimage*,int,int);
    //uint32_t (*createStandardCursor)(_wManagercursor*,int);
    //void (*destroyCursor)(_wManagercursor*);
    //void (*setCursor)(wManagerWindow*,_wManagercursor*);
    const char* (*getScancodeName)(int);
    int (*getKeyScancode)(int);
    void (*setClipboardString)(const char*);
    const char* (*getClipboardString)(void);
    //uint32_t (*initJoysticks)(void);
    //void (*terminateJoysticks)(void);
    //uint32_t (*pollJoystick)(_wManagerjoystick*,int);
    //const char* (*getMappingName)(void);
    //void (*updateGamepadGUID)(char*);
    // monitor
    //void (*freeMonitor)(_wManagerMonitor*);
    //void (*getMonitorPos)(_wManagerMonitor*,int*,int*);
    //void (*getMonitorContentScale)(_wManagerMonitor*,float*,float*);
    //void (*getMonitorWorkarea)(_wManagerMonitor*,int*,int*,int*,int*);
    //wManagerVidmode* (*getVideoModes)(_wManagerMonitor*,int*);
    //void (*getVideoMode)(_wManagerMonitor*,wManagerVidmode*);
    //uint32_t (*getGammaRamp)(_wManagerMonitor*,wManagerGammaramp*);
    //void (*setGammaRamp)(_wManagerMonitor*,const wManagerGammaramp*);
    // window
    uint32_t (*createWindow)(wManagerWindow*,const _wManagerwndconfig*,const _wManagerfbconfig*);
    void (*destroyWindow)(wManagerWindow*);
    void (*setWindowTitle)(wManagerWindow*,const char*);
    //void (*setWindowIcon)(wManagerWindow*,int,const wManagerimage*);
    void (*getWindowPos)(wManagerWindow*,int*,int*);
    void (*setWindowPos)(wManagerWindow*,int,int);
    void (*getWindowSize)(wManagerWindow*,int*,int*);
    void (*setWindowSize)(wManagerWindow*,int,int);
    void (*setWindowSizeLimits)(wManagerWindow*,int,int,int,int);
    void (*setWindowAspectRatio)(wManagerWindow*,int,int);
    void (*getFramebufferSize)(wManagerWindow*,int*,int*);
    void (*getWindowFrameSize)(wManagerWindow*,int*,int*,int*,int*);
    void (*getWindowContentScale)(wManagerWindow*,float*,float*);
    void (*iconifyWindow)(wManagerWindow*);
    void (*restoreWindow)(wManagerWindow*);
    void (*maximizeWindow)(wManagerWindow*);
    void (*showWindow)(wManagerWindow*);
    void (*hideWindow)(wManagerWindow*);
    void (*requestWindowAttention)(wManagerWindow*);
    void (*focusWindow)(wManagerWindow*);
    //void (*setWindowMonitor)(wManagerWindow*,_wManagerMonitor*,int,int,int,int,int);
    uint32_t (*windowFocused)(wManagerWindow*);
    uint32_t (*windowIconified)(wManagerWindow*);
    uint32_t (*windowVisible)(wManagerWindow*);
    uint32_t (*windowMaximized)(wManagerWindow*);
    uint32_t (*windowHovered)(wManagerWindow*);
    uint32_t (*framebufferTransparent)(wManagerWindow*);
    float (*getWindowOpacity)(wManagerWindow*);
    void (*setWindowResizable)(wManagerWindow*,uint32_t);
    void (*setWindowDecorated)(wManagerWindow*,uint32_t);
    void (*setWindowFloating)(wManagerWindow*,uint32_t);
    void (*setWindowOpacity)(wManagerWindow*,float);
    void (*setWindowMousePassthrough)(wManagerWindow*,uint32_t);
    void (*pollEvents)(void);
    void (*waitEvents)(void);
    void (*waitEventsTimeout)(double);
    void (*postEmptyEvent)(void);
    // EGL
    /*EGLenum (*getEGLPlatform)(EGLint**);
    EGLNativeDisplayType (*getEGLNativeDisplay)(void);
    EGLNativeWindowType (*getEGLNativeWindow)(wManagerWindow*);*/
    // vulkan
    void (*getRequiredInstanceExtensions)(char**);
    //uint32_t (*getPhysicalDevicePresentationSupport)(VkInstance,VkPhysicalDevice,uint32_t);
    //VkResult (*createWindowSurface)(VkInstance,wManagerWindow*,const VkAllocationCallbacks*,VkSurfaceKHR*);
} _wManagerPlatform;

#endif
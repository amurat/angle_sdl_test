#include <glad/glad_egl.h>
/*
#include <dlfcn.h>
#define DEFAULT_EGL "libEGL.dylib"

typedef EGLDisplay (EGLAPIENTRYP PFNEGLGETDISPLAYPROC) (EGLNativeDisplayType display_id);
typedef EGLBoolean (EGLAPIENTRYP PFNEGLINITIALIZEPROC) (EGLDisplay dpy, EGLint *major, EGLint *minor);

static PFNEGLGETDISPLAYPROC eglGetDisplayFn;
static PFNEGLINITIALIZEPROC eglInitializeFn;

static void setup()
{
    auto dll = dlopen(DEFAULT_EGL, 0);
    eglGetDisplayFn = (PFNEGLGETDISPLAYPROC)dlsym(dll, "eglGetDisplay");
    eglInitializeFn = (PFNEGLINITIALIZEPROC)dlsym(dll, "eglInitialize");
}
*/

static EGLDisplay display;
static EGLSurface surface;

void EndEGLFrame()
{
    eglSwapBuffers(display, surface);
}

bool SetupEGL(void* nw)
{
  //  setup();
    
    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLContext context;
    
    EGLConfig config;
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

    EGLint configAttribs[] =
        {
          EGL_RED_SIZE, 8,
          EGL_GREEN_SIZE, 8,
          EGL_BLUE_SIZE, 8,
          EGL_ALPHA_SIZE, 8,
          EGL_DEPTH_SIZE, 16,
          EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
          EGL_NONE
        };
    
    // Get Display
#if 1
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if ( display == EGL_NO_DISPLAY )
    {
        return false;
    }
#else
    EGLenum platform = EFL_NONE;
    eglGetPlatformDisplay(EGL_DEFAULT_DISPLAY)
    std::vector<EGLAttrib> displayAttributes;
    displayAttributes.push_back(EGL_PLATFORM_ANGLE_TYPE_OPENGL_ANGLE);
    displayAttributes.push_back(EGL_PLATFORM_ANGLE_TYPE_DEFAULT_ANGLE);
    displayAttributes.push_back(EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE);
    displayAttributes.push_back(EGL_DONT_CARE);
    displayAttributes.push_back(EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE);
    displayAttributes.push_back(EGL_DONT_CARE);
    displayAttributes.push_back(EGL_NONE);
    display = eglGetPlatformDisplay(EGL_PLATFORM_ANGLE_ANGLE,
                                    reinterpret_cast<void *>(0),
                                    &displayAttributes[0]);
    if (eglGetError() != EGL_SUCCESS)
    {
        std::cerr << "Error on eglGetPlatformDisplay.\n";
        return false;
    }
#endif

    // Initialize EGL
    if ( !eglInitialize(display, &majorVersion, &minorVersion) )
    {
        return false;
    }
#if 1
    if ( !eglBindAPI(EGL_OPENGL_ES_API) )
    {
        return false;
    }

    // Get configs
    if ( !eglGetConfigs(display, 0, 0, &numConfigs) )
    {
        return false;
    }
    

    // Choose config
    if ( !eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) )
    {
        return false;
    }
    
    // Create a surface
    
    surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)nw, 0);
    if ( surface == EGL_NO_SURFACE )
    {
        return false;
    }
    
    // Create a GL context
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
    if ( context == EGL_NO_CONTEXT )
    {
        return false;
    }
    
    // Make the context current
    if ( !eglMakeCurrent(display, surface, surface, context) )
    {
        return false;
    }
    
    //*eglDisplay = display;
    //*eglSurface = surface;
    //*eglContext = context;
#endif
    return EGL_TRUE;
}

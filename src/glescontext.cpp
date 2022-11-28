#include "glescontext.h"

GLESContext::GLESContext(void* nativeWindowHandle) :
    display_(0),
    surface_(0),
    context_(0),
    nw_(nativeWindowHandle)
{
}

GLESContext::~GLESContext()
{
    if (surface_) {
        eglDestroySurface(display_, surface_);
        surface_ = 0;
        eglTerminate(display_);
        display_ = 0;
    }
}

void GLESContext::swapBuffers()
{
    eglSwapBuffers(display_, surface_);
}

bool GLESContext::create()
{
  //  setup();
    
    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    
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
    display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if ( display_ == EGL_NO_DISPLAY )
    {
        return false;
    }

    // Initialize EGL
    if ( !eglInitialize(display_, &majorVersion, &minorVersion) )
    {
        return false;
    }

    if ( !eglBindAPI(EGL_OPENGL_ES_API) )
    {
        return false;
    }

    // Get configs
    if ( !eglGetConfigs(display_, 0, 0, &numConfigs) )
    {
        return false;
    }
    
    // Choose config
    if ( !eglChooseConfig(display_, configAttribs, &config, 1, &numConfigs) )
    {
        return false;
    }
    
    // Create a surface
    surface_ = eglCreateWindowSurface(display_, config, nw_, 0);
    if ( surface_ == EGL_NO_SURFACE )
    {
        return false;
    }
    
    // Create a GL context
    context_ = eglCreateContext(display_, config, EGL_NO_CONTEXT, contextAttribs );
    if ( context_ == EGL_NO_CONTEXT )
    {
        return false;
    }
    
    // Make the context current
    if ( !eglMakeCurrent(display_, surface_, surface_, context_) )
    {
        return false;
    }
    return true;
}

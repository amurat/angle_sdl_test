#include <glad/glad_egl.h>

static EGLint const attribute_list[] = {
        EGL_RED_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_NONE
};

void InitEGLContext()
{
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    EGLSurface surface;
    NativeWindowType native_window;
    EGLint num_config;
    
    /* get an EGL display connection */
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    
    /* initialize the EGL display connection */
    eglInitialize(display, 0, 0);
    
    /* get an appropriate EGL frame buffer configuration */
    eglChooseConfig(display, attribute_list, &config, 1, &num_config);
    
    /* create an EGL rendering context */
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, 0);
}

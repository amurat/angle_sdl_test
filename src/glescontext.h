#pragma once

#include <glad/glad_egl.h>

class GLESContext {
public:
    GLESContext(void* nativeWindowHandle);
    virtual ~GLESContext();
    
    bool create();
    void swapBuffers();
private:
    EGLNativeWindowType nw_;
    EGLDisplay display_;
    EGLSurface surface_;
    EGLContext context_;
};

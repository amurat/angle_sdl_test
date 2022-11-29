#include <SDL.h>
#include <SDL_syswm.h>

#include <iostream>
#include <assert.h>

#include "macwindow.h"

extern void SetupGLES2Renderer();
extern void RenderGLES2Renderer(int w, int h);

extern void SetupGL2Renderer();
extern void RenderGL2Renderer(int w, int h);

extern bool SetupEGL(void* nativeWindowHandle);
extern void EndEGLFrame();

int main(int, char**) {
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    std::cerr << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }

  SDL_version version;
  SDL_GetVersion(&version);
  std::cout << "SDL version: " << static_cast<int>(version.major) << "."
            << static_cast<int>(version.minor) << "."
            << static_cast<int>(version.patch) << std::endl;

  setenv("GALOGEN_GL4ES_LIBRARY", "libGL4ES.dylib", 1);
    
  // Create window
  const bool bUseGLSDL = false;
  const bool bInitGLES = true;
<<<<<<< HEAD
  const bool bRenderGLES = false;
  const bool bInitSDLContext = true;
    
  if (bInitGLES) {
      SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_EGL, 1);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  } else {
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
=======
  const bool bRenderGLES = true;
    
    if (bUseGLSDL) {
          if (bInitGLES) {
              SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
              SDL_GL_SetAttribute(SDL_GL_CONTEXT_EGL, 1);
              SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
          } else {
              SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
              SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
              SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
          }
        // Explicitly set channel depths, otherwise we might get some < 8
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    }
    

  auto windowFlags = SDL_WINDOW_SHOWN;
  if (bUseGLSDL)
  {
      windowFlags = (SDL_WindowFlags)(SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
>>>>>>> db79eff8230b7be00f010effc90986398b2de420
  }
    
  int width = 512;
  int height = 512;
  auto window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
    
    if (true) { //(!bUseGLSDL) {
        void* nativeWindowHandle = 0;

        SDL_SysWMinfo info;
        SDL_VERSION(&info.version); /* initialize info structure with SDL version info */

        if (SDL_GetWindowWMInfo(window, &info)) {
#ifdef __APPLE__
            if (SDL_SYSWM_COCOA == info.subsystem) {
                nativeWindowHandle = GetNativeWindowHandle(info.info.cocoa.window);
                std::cout << "Cocoa\n";
                GetWindowDrawableSize(info.info.cocoa.window, &width, &height);
                std::cout << "w : " << width << " h : " << height << std::endl;
            }
#endif

#ifdef __LINUX__
            if (SDL_SYSWM_X11 == info.subsystem) {
                nativeWindowHandle = (void*)info.info.x11.window;
                std::cout << "X11\n";
            }
#endif
        }
        
        if (!SetupEGL(nativeWindowHandle)) {
            assert(false && "SetEGL failed");
        }
    } else {
        auto glContext = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, glContext);
    }
    
  // Init GL
<<<<<<< HEAD
    SDL_GLContext glContext = 0;
    if (bInitSDLContext) {
        glContext = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, glContext);
    }
=======
>>>>>>> db79eff8230b7be00f010effc90986398b2de420
  if (bRenderGLES) {
      SetupGLES2Renderer();
  } else {
      SetupGL2Renderer();
  }
    
    // Main loop
    bool isRunning = true;
    while (isRunning) {
        SDL_Event event;
        while (0 != SDL_PollEvent(&event)) {
            if (SDL_QUIT == event.type) {
                isRunning = false;
            } else if (SDL_KEYDOWN == event.type) {
                const auto keyStates_p = SDL_GetKeyboardState(nullptr);
                if (keyStates_p[SDL_SCANCODE_ESCAPE]) {
                    isRunning = false;
                }
            }
        }
        if (bRenderGLES) {
            RenderGLES2Renderer(width, height);
        } else {
            RenderGL2Renderer(width, height);
        }
        EndEGLFrame();
    }

    // Clean up
<<<<<<< HEAD
  if (bInitSDLContext) {
    SDL_GL_DeleteContext(glContext);
    glContext = 0;
  }
=======
  //SDL_GL_DeleteContext(glContext);
>>>>>>> db79eff8230b7be00f010effc90986398b2de420
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}

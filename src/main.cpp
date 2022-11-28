#include <SDL.h>
#include <SDL_syswm.h>

#include <iostream>
#include <assert.h>

#include "macwindow.h"
#include "glescontext.h"
#include "rendergl.h"

extern void SetupGLES2Renderer();
extern void RenderGLES2Renderer(int w, int h);

extern void SetupGL2Renderer();
extern void RenderGL2Renderer(int w, int h);


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
    const bool bInitGLES = true;
    const bool bRenderGLES = true;
    
    auto windowFlags = SDL_WINDOW_SHOWN;
    int width = 512;
    int height = 512;
    auto window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
    
    GLESContext* glesContext = 0;
    
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
    
    glesContext = new GLESContext(nativeWindowHandle);
    if (!glesContext->create()) {
        assert(false && "glesContext.create failed");
    }

    RenderGL* rendergl = 0;
    // Init GL
    if (bRenderGLES) {
        rendergl = new RenderGLES2();
    } else {
        rendergl = new RenderGL2();
    }
    rendergl->setup();
    
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
        rendergl->render(width, height);
        glesContext->swapBuffers();
    }
    
    // Clean up
    delete glesContext;
    glesContext = 0;
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

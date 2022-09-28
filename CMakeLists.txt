cmake_minimum_required(VERSION 3.13)

project(angle_sdl_test)

# Module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(sdl_PREFIX "${CMAKE_BINARY_DIR}/sdl")
set(sdl_INSTALL_DIR "${CMAKE_BINARY_DIR}/sdl")
set(sdl_DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/sdl/download")
set(sdl_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${sdl_INSTALL_DIR} -DSDL_SHARED=OFF)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(sdl_CMAKE_ARGS "${sdl_CMAKE_ARGS}" -DFORCE_STATIC_VCRT=ON -DHAVE_LIBC=ON)
endif()
set(sdl_URL https://www.libsdl.org/release/SDL2-2.0.12.zip)
set(SDL_SHARED OFF CACHE BOOL "sdl shared")
set(SDL_SHARED_ENABLED_BY_DEFAULT OFF CACHE BOOL "sdl share enabled by default")
include(ExternalProject)
ExternalProject_Add(SDL
    PREFIX ${sdl_PREFIX}
    URL ${sdl_URL}
    INSTALL_DIR ${sdl_INSTALL_DIR}
    CMAKE_ARGS ${sdl_CMAKE_ARGS}
)

include_directories(
    ${CMAKE_BINARY_DIR}/sdl/include/SDL2
)
link_directories("${CMAKE_BINARY_DIR}/sdl/lib")

set(OSX_FRAMEWORKS
    Foundation
    AppKit
    ApplicationServices
    CoreVideo
    CoreAudio
    AudioToolbox
    AudioUnit
    IOKit
    ForceFeedback
    Carbon
    Metal
)


foreach(FW ${OSX_FRAMEWORKS})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework ${FW}")
endforeach()


# Sources
set(SOURCE_FILES src/main.cpp src/rendergles2.cpp src/rendergl2.cpp
    vendor/galogen/gl.c)

# Dependencies
if(WIN32)
    set(EGL_LIBRARY "${PROJECT_SOURCE_DIR}/lib/win64/angle/libEGL.lib")
    set(GLESV2_LIBRARY "${PROJECT_SOURCE_DIR}/lib/win64/angle/libGLESv2.lib")
    set(SDL2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/vendor/sdl/include/sdl2")
    set(SDL2_LIBRARY
        "${PROJECT_SOURCE_DIR}/lib/win64/sdl2/SDL2.lib"
        "${PROJECT_SOURCE_DIR}/lib/win64/sdl2/SDL2main.lib")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  if (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "arm64")
    set(EGL_LIBRARY "${PROJECT_SOURCE_DIR}/lib/macos-arm64/angle/libEGL.dylib")
    set(GLESV2_LIBRARY "${PROJECT_SOURCE_DIR}/lib/macos-arm64/angle/libGLESv2.dylib")
    set(GL4ES_LIBRARY "${PROJECT_SOURCE_DIR}/lib/macos-arm64/gl4es/libGL.dylib")
  else()
    set(EGL_LIBRARY "${PROJECT_SOURCE_DIR}/lib/macos/angle/libEGL.dylib")
    set(GLESV2_LIBRARY "${PROJECT_SOURCE_DIR}/lib/macos/angle/libGLESv2.dylib")
    set(GL4ES_LIBRARY "${PROJECT_SOURCE_DIR}/lib/macos/gl4es/libGL.dylib")
  endif()
endif()

# Executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Target properties
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${SDL2_INCLUDE_DIR}
  src/
  vendor/angle/include/
  vendor/
)

target_link_libraries(${PROJECT_NAME}
  ${EGL_LIBRARY}
  ${GLESV2_LIBRARY}
)

#target_link_libraries(${PROJECT_NAME}
#  ${SDL2_LIBRARY}
#)

target_link_libraries(${PROJECT_NAME} debug SDL2d optimized SDL2)
target_link_libraries(${PROJECT_NAME} iconv)

add_dependencies(${PROJECT_NAME} SDL)

# Dynamic libraries
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/lib/win64/angle/libEGL.dll"
        "${PROJECT_SOURCE_DIR}/lib/win64/angle/libGLESv2.dll"
        "${PROJECT_SOURCE_DIR}/lib/win64/sdl2/SDL2.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${EGL_LIBRARY}"
        "${GLESV2_LIBRARY}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GL4ES_LIBRARY}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/libGL4ES.dylib)

endif()

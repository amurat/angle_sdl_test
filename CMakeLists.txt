cmake_minimum_required(VERSION 3.13)

project(angle_sdl_test)

# Module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Sources
set(SOURCE_FILES src/main.cpp)

# Dependencies
if(WIN32)
    set(EGL_LIBRARY "${PROJECT_SOURCE_DIR}/lib/win64/angle/libEGL.lib")
    set(GLESV2_LIBRARY "${PROJECT_SOURCE_DIR}/lib/win64/angle/libGLESv2.lib")
    set(SDL2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/vendor/sdl/include/sdl2")
    set(SDL2_LIBRARIES
        "${PROJECT_SOURCE_DIR}/lib/win64/sdl2/SDL2.lib"
        "${PROJECT_SOURCE_DIR}/lib/win64/sdl2/SDL2main.lib")
else()
    set(EGL_LIBRARY "${PROJECT_SOURCE_DIR}/lib/macos/angle/libEGL.dylib")
    set(GLESV2_LIBRARY "${PROJECT_SOURCE_DIR}/lib/macos/angle/libGLESv2.dylib")
    find_package(SDL2 REQUIRED)
endif()

# Executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Target properties
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${SDL2_INCLUDE_DIR}
  src/
  vendor/angle/include/
)

target_link_libraries(${PROJECT_NAME}
  ${EGL_LIBRARY}
  ${GLESV2_LIBRARY}
  ${SDL2_LIBRARY}
)

target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTIONS})

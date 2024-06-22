cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

project(main)

find_package(assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})

add_subdirectory(external/glfw)
add_subdirectory(external/glm)

add_executable(main 
    main.cpp

    src/logger.cpp
    src/window.cpp
    src/raw_model.cpp
    src/loader.cpp
    src/renderer.cpp
    src/shader_program.cpp
    src/static_shader.cpp
    src/model_texture.cpp
    src/textured_model.cpp
    src/entity.cpp
    src/math.cpp
    src/camera.cpp
    src/obj_loader.cpp


    external/glad/src/glad.c
)

target_include_directories(main
    
    # PRIVATE external/glfw/include # No need cause it is getting included automatically when we are linking to it
    PRIVATE external/glm

    PRIVATE include
    PRIVATE external/glad/include
    PRIVATE external/stbImage/include
)

target_link_libraries(main
    PRIVATE glfw
    PRIVATE assimp::assimp
)
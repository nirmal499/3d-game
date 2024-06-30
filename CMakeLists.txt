cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

project(main)

find_package(assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})

set(Boost_ROOT "/home/nbaskey/my_opt/boost_libs/")
find_package(Boost 1.84 REQUIRED COMPONENTS)

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
    src/light.cpp
    src/master_renderer.cpp
    src/player.cpp

    src/client.cpp
    src/server.cpp

    external/glad/src/glad.c
)

add_executable(server 
    server_main.cpp
    src/server.cpp
)

target_include_directories(main
    
    # PRIVATE external/glfw/include # No need cause it is getting included automatically when we are linking to it
    PRIVATE external/glm

    PRIVATE include
    PRIVATE external/glad/include
    PRIVATE external/stbImage/include
    PRIVATE ${Boost_INCLUDE_DIRS}
)

target_link_libraries(main
    PRIVATE glfw
    PRIVATE assimp::assimp
    PRIVATE ${Boost_SYSTEM_LIBRARY}
)

target_include_directories(server
    PRIVATE include
    PRIVATE ${Boost_INCLUDE_DIRS}
)

target_link_libraries(server
    PRIVATE ${Boost_SYSTEM_LIBRARY}
)
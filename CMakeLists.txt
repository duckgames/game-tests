cmake_minimum_required(VERSION 3.9)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
SET_SOURCE_FILES_PROPERTIES(system.cpp PROPERTIES COMPILE_FLAGS -O0)
find_package(Lua REQUIRED)

include_directories(${LUA_INCLUDE_DIRS})

## Use raylib_main.cpp
add_executable(untitled raylib_main.cpp lib/raylib/raylib.h components.h system.cpp system.h world.h world.cpp controller.h EntityPool.cpp EntityPool.h TextureAtlasLocation.h)
target_link_libraries(untitled raylib jsoncpp ${LUA_LIBRARIES})

## Use sfml_main.cpp
#add_executable(untitled sfml_main.cpp background.h background.cpp following-background.cpp following-background.h components.h system.cpp system.h world.h world.cpp controller.h EntityPool.cpp EntityPool.h TextureAtlasLocation.h)
#target_link_libraries(untitled sfml-graphics sfml-window sfml-system jsoncpp ${LUA_LIBRARIES})
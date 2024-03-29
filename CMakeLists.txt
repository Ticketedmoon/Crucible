cmake_minimum_required(VERSION 3.26.4...3.28.1)
project(crucible)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(IS_RELEASE_MODE FALSE)

# Note: This compiler optimisation may break debugging.
if (IS_RELEASE_MODE)
    message("-- Loading release build, compiler optimisations set")
    set(CXXFLAGS  "-Wall -std=c++${CMAKE_CXX_STANDARD} -I./include -I./lib/header -g -O2")
else()
    message("-- Loading debug build, compiler optimisations disabled")
    set(CXXFLAGS  "-Wall -std=c++${CMAKE_CXX_STANDARD} -I./include -I./lib/header -g -O0")
endif ()
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")

message("-- C++ Version Found " = " ${CMAKE_CXX_STANDARD}")

# The term "build type" in CMake refers to a certain build configuration such as debug and release.
# We give our configuration a debug build type by default.
# A debug build often results in fast compile times with debugging symbols baked
#  into executables - enabling debugging with build-tools as such as GDB and LLDB:

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif ()

# command generates a header file called config.h which includes preprocessor definitions that specify
#  our project's major and minor version number. CMake reads config.h.in to produce this header file:
# configure_file(config.h.in config.h)

file(GLOB ROOT_GAME_SOURCES "src/*.cpp")

file(GLOB CORE_GAME_SOURCES "src/core/*.cpp")
file(GLOB CORE_ENGINE_GAME_SOURCES "src/core/engine/*.cpp")
file(GLOB CORE_LEVEL_GAME_SOURCES "src/core/level/*.cpp")
file(GLOB CORE_MANAGER_GAME_SOURCES "src/core/manager/*.cpp")
file(GLOB CORE_VIEW_GAME_SOURCES "src/core/view/*.cpp")

file(GLOB SCENE_SOURCES "src/scene/*.cpp")
file(GLOB ENTITY_SOURCES "src/entity/*.cpp")
file(GLOB SYSTEM_SOURCES "src/system/*.cpp")

file(GLOB VICTORY_SCENE_ROOT_COMPONENT_SOURCES "src/scene/victory_scene/*.cpp")
file(GLOB VICTORY_SCENE_SYSTEM_SOURCES "src/scene/victory_scene/system/*.cpp")

file(GLOB GAMEPLAY_SCENE_ROOT_COMPONENT_SOURCES "src/scene/gameplay_scene/*.cpp")
file(GLOB GAMEPLAY_SCENE_COMPONENT_SOURCES "src/scene/gameplay_scene/component/*.cpp")
file(GLOB GAMEPLAY_SCENE_ENTITY_SOURCES "src/scene/gameplay_scene/entity/*.cpp")
file(GLOB GAMEPLAY_SCENE_SYSTEM_SOURCES "src/scene/gameplay_scene/system/*.cpp")

file(COPY resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# Detect and add SFML
find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)

add_executable(${PROJECT_NAME} ${ROOT_GAME_SOURCES}
        ${SCENE_SOURCES} ${ENTITY_SOURCES} ${SYSTEM_SOURCES}
        # Core
        ${CORE_GAME_SOURCES} ${CORE_ENGINE_GAME_SOURCES} ${CORE_LEVEL_GAME_SOURCES}
        ${CORE_MANAGER_GAME_SOURCES} ${CORE_VIEW_GAME_SOURCES}
        # Gameplay
        ${GAMEPLAY_SCENE_ROOT_COMPONENT_SOURCES} ${GAMEPLAY_SCENE_COMPONENT_SOURCES} ${GAMEPLAY_SCENE_ENTITY_SOURCES} ${GAMEPLAY_SCENE_SYSTEM_SOURCES}
        # Victory
        ${VICTORY_SCENE_ROOT_COMPONENT_SOURCES} ${VICTORY_SCENE_SYSTEM_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE include
        include/component include/entity include/system include/scene
        include/core include/core/engine, include/core/level, include/core/manager, include/core/view
        include/scene/gameplay_scene include/scene/gameplay_scene/component include/scene/gameplay_scene/system
        include/scene/victory_scene include/scene/victory_scene/system)
target_include_directories(${PROJECT_NAME} PRIVATE lib/header)

target_link_libraries(${PROJECT_NAME} PUBLIC sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

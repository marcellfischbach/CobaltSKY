
cmake_minimum_required(VERSION 3.3)


project(CobaltSKY)



set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CobaltSKY_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CobaltSKY_BINARY_DIR}/bin)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
unset(CMAKE_CXX_CL_SHOWINCLUDE)


include(${CobaltSKY_SOURCE_DIR}/Scripts/CobaltSKY.cmake) 


if (WIN32)
message("Use Win32")
add_definitions(-DCS_WIN32)
else()
message("Use Linux")
add_definitions(-DCS_LINUX)
endif()

string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER )
if (CMAKE_BUILD_TYPE_LOWER STREQUAL "debug")
    add_definitions(-D_DEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

find_package(OpenGL)
find_package(PNG)
find_package(Bullet)
find_package(GLEW)

include(${CobaltSKY_SOURCE_DIR}/Scripts/Findassimp.cmake)
#include(${CobaltSKY_SOURCE_DIR}/Scripts/FindGLEW.cmake)
include(${CobaltSKY_SOURCE_DIR}/Scripts/FindSDL2.cmake)
#include(${CobaltSKY_SOURCE_DIR}/Scripts/FindPhysX3.cmake)
#include(${CobaltSKY_SOURCE_DIR}/Scripts/FindBullet.cmake)

include_directories(${SDL_INCLUDE_DIR})
include_directories(${PNG_INCLUDE_DIRS})
include_directories(${BULLET_INCLUDE_DIRS})
include_directories(${CobaltSKY_SOURCE_DIR}/Engine)
#include_directories(${CobaltSKY_BINARY_DIR}/Engine)
include_directories(${CobaltSKY_SOURCE_DIR}/Editor)
#include_directories(${CobaltSKY_BINARY_DIR}/Editor)


#add_subdirectory(3rdparty)
#add_subdirectory(ClassGenerator)
add_subdirectory(Engine)
add_subdirectory(moc)
add_subdirectory(Editor)


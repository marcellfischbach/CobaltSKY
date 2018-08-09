
find_package(Qt5 COMPONENTS Core Widgets OpenGL Xml REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CobaltSKY_SOURCE_DIR}/Tools/EditorLauncher)
include_directories(${CobaltSKY_BINARY_DIR}/Tools/EditorLauncher)
set(CMAKE_AUTOMOC ON)

set(ALL_SOURCES
    main.cc
    )


add_executable(
    EditorLauncher
    ${ALL_SOURCES}
    )


target_link_libraries(EditorLauncher
    Qt5::Core
    Qt5::Widgets
    Qt5::OpenGL
    Qt5::Xml
    Editor
    )

CS_INSTALL_MOCCER(EditorLauncher NO_PREFIX NO_API ALL_SOURCES)

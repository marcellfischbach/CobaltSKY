
find_package(Qt5 COMPONENTS Core Widgets OpenGL Xml REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})

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
	EditorSampler
	EditorTexture
    )

CS_INSTALL_MOCCER(EditorLauncher editorlauncher NO_API ALL_SOURCES)

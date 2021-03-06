
find_package(Qt5 COMPONENTS Core Widgets OpenGL Xml REQUIRED)

include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})

add_definitions(-DTEXTUREEDITOR_EXPORT)

set(CMAKE_AUTOMOC ON)


set(EDITORS_TEXTUREEDITOR_SOURCES
    textureeditor.cc
    textureeditor.hh
    textureeditorfactory.cc
    textureeditorfactory.hh
    textureeditormodule.cc
    textureeditormodule.hh
    textureeditorproperties.cc
    textureeditorproperties.hh
    textureeditorrenderwidget.cc
    textureeditorrenderwidget.hh
    textureeditorwidget.cc
    textureeditorwidget.hh
    )

set(EDITORS_TEXTUREEDITOR_UI
    textureeditorwidget.ui
    )


set(ALL_UI
    ${EDITORS_TEXTUREEDITOR_UI}
    )


qt5_wrap_ui(ALL_UI_SOURCES ${ALL_UI})
qt5_add_resources(RESOURCES_SOURCES resources.qrc)

set(ALL_SOURCES
    ${EDITORS_TEXTUREEDITOR_SOURCES}
    )


add_library(Editor-TextureEditor SHARED
    ${ALL_SOURCES}
    ${ALL_UI_SOURCES}
    ${RESOURCES_SOURCES}
    )


target_link_libraries(Editor-TextureEditor
    Editor
    )

CS_INSTALL_MOCCER(Editor-TextureEditor textureeditor TEXTUREEDITOR_API ALL_SOURCES)

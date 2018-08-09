
find_package(Qt5 COMPONENTS Core Widgets OpenGL Xml REQUIRED)

include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})


set(EDITORS_MATERIALEDITOR_SOURCES
    materialeditor.cc
    materialeditor.hh
    materialeditorfactory.cc
    materialeditorfactory.hh
    materialeditornew.cc
    materialeditornew.hh
    materialeditorpreviewsceneview.cc
    materialeditorpreviewsceneview.hh
    materialeditorproperties.cc
    materialeditorproperties.hh
    materialeditorwidget.cc
    materialeditorwidget.hh
    )

set(EDITORS_MATERIALEDITOR_UI
    materialeditorwidget.ui
    )

set(ALL_UI
    ${EDITORS_MATERIALEDITOR_UI}
    )


qt5_wrap_ui(ALL_UI_SOURCES ${ALL_UI})
qt5_add_resources(RESOURCES_SOURCES resources.qrc)

set(ALL_SOURCES
    ${EDITORS_MATERIALEDITOR_SOURCES}
    )

add_library(EditorMaterial SHARED
    ${ALL_SOURCES}
    ${ALL_UI_SOURCES}
    ${RESOURCES_SOURCES}
    )


target_link_libraries(EditorMaterial
    Editor
    )

CS_INSTALL_MOCCER(EditorMaterial NO_PREFIX NO_API ALL_SOURCES)


find_package(Qt5 COMPONENTS Core Widgets OpenGL Xml REQUIRED)

include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})

add_definitions(-DSHADERGRAPHEDITOR_EXPORT)

set(CMAKE_AUTOMOC ON)

set(EDITORS_SHADERGRAPHEDITOR_SOURCES
    shadergrapheditor.cc
    shadergrapheditor.hh
    shadergrapheditorfactory.cc
    shadergrapheditorfactory.hh
    shadergrapheditormeta.cc
    shadergrapheditormeta.hh
    shadergrapheditormetaassetcsfloader.cc
    shadergrapheditormetaassetcsfloader.hh
    shadergrapheditormodule.cc
    shadergrapheditormodule.hh
    shadergrapheditornew.cc
    shadergrapheditornew.hh
    shadergrapheditornode.cc
    shadergrapheditornode.hh
    shadergrapheditoroutliner.cc
    shadergrapheditoroutliner.hh
    shadergrapheditoroutlinertablemodel.cc
    shadergrapheditoroutlinertablemodel.hh
    shadergrapheditorpreview.cc
    shadergrapheditorpreview.hh
    shadergrapheditorpreviewsceneview.cc
    shadergrapheditorpreviewsceneview.hh
    shadergrapheditorproperties.cc
    shadergrapheditorproperties.hh
    shadergrapheditorshadergraphproperties.cc
    shadergrapheditorshadergraphproperties.hh
    shadergrapheditortoolbox.cc
    shadergrapheditortoolbox.hh
    shadergrapheditortoolboxmodel.cc
    shadergrapheditortoolboxmodel.hh
    shadergrapheditorwidget.cc
    shadergrapheditorwidget.hh
    )

set(EDITORS_SHADERGRAPHEDITOR_UI
    shadergrapheditoroutliner.ui
    shadergrapheditorpreview.ui
    shadergrapheditorshadergraphproperties.ui
    shadergrapheditortoolbox.ui
    shadergrapheditorwidget.ui
    )

set(ALL_UI
    ${EDITORS_SHADERGRAPHEDITOR_UI}
    )


qt5_wrap_ui(ALL_UI_SOURCES ${ALL_UI})
qt5_add_resources(RESOURCES_SOURCES resources.qrc)

set(ALL_SOURCES
    ${EDITORS_SHADERGRAPHEDITOR_SOURCES}
    )

add_library(Editor-ShaderGraphEditor SHARED
    ${ALL_SOURCES}
    ${ALL_UI_SOURCES}
    ${RESOURCES_SOURCES}
    )


target_link_libraries(Editor-ShaderGraphEditor
    Editor
    )

CS_INSTALL_MOCCER(Editor-ShaderGraphEditor shadergrapheditor SHADERGRAPHEDITOR_API ALL_SOURCES)


find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
find_package(Qt5Xml)
find_package(assimp)

include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

set(EDITOR_SOURCES
    abstractasseteditor.cc
    abstractasseteditor.hh
    assetdescriptor.cc
    assetdescriptor.hh
    main.cc
    editor.cc
    editor.hh
    editormodule.cc
    editormodule.hh
    iasseteditor.hh
    iasseteditorfactory.hh
    mainwindow.cc
    mainwindow.hh
    )

source_group(
    "editor"
    FILES
    ${EDITOR_SOURCES}
    )

set(ASSETMANAGER_SOURCES
    assetmanager/assetmanagercontentmodel.cc
    assetmanager/assetmanagercontentmodel.hh
    assetmanager/assetmanagerdock.cc
    assetmanager/assetmanagerdock.hh
    assetmanager/assetmanagerfoldermodel.cc
    assetmanager/assetmanagerfoldermodel.hh
    assetmanager/assetmanagerwidget.cc
    assetmanager/assetmanagerwidget.hh
    )

set(ASSETMANAGER_UI
    assetmanager/assetmanagerwidget.ui
    )

source_group(
    "editor\\assetmanager"
    FILES
    ${ASSETMANAGER_SOURCES}
    ${ASSETMANAGER_UI}
    )



set(SAMPLEREDITOR_SOURCES
    samplereditor/samplereditor.cc
    samplereditor/samplereditor.hh
    samplereditor/samplereditorfactory.cc
    samplereditor/samplereditorfactory.hh
    samplereditor/samplereditorwidget.cc
    samplereditor/samplereditorwidget.hh
    )

set(SAMPLEREDITOR_UI
    samplereditor/samplereditorwidget.ui
    )


source_group(
    "editor\\samplereditor"
    FILES
    ${SAMPLEREDITOR_SOURCES}
    ${SAMPLEREDITOR_UI}
    )



set(ALL_UI
    ${ASSETMANAGER_UI}
    ${SAMPLEREDITOR_UI}
    )

qt5_wrap_ui(ALL_UI_SOURCES ${ALL_UI})
qt5_add_resources(RESOURCES_SOURCES resources.qrc)

set(ALL_SOURCES
    ${EDITOR_SOURCES}
    ${ASSETMANAGER_SOURCES}
    ${SAMPLEREDITOR_SOURCES}
    )

add_executable(
    Editor
    ${ALL_SOURCES}
    ${ALL_UI_SOURCES}
    ${RESOURCES_SOURCES}
    )

target_link_libraries(Editor
    Qt5::Core
    Qt5::Widgets
    Qt5::OpenGL
    Qt5::Xml
    ValkyrieEngine
    ValkyrieEngine-GraphicsGL4
    ValkyrieEngine-PhysicsBullet
    ${ASSIMP_LIBRARY}
    )

VK_INSTALL_MOCCER(Editor NO_PREFIX NO_API ALL_SOURCES)

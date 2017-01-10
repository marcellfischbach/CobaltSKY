
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
    main.cc
    editor.cc
    editor.hh
    mainwindow.cc
    mainwindow.hh
    )

source_group(
    "Editor"
    FILES
    ${EDITOR_SOURCES}
    )

set(ASSETMANAGER_SOURCES
    assetmanager/assetmanagerdock.cc
    assetmanager/assetmanagerdock.hh
    assetmanager/assetmanagerwidget.cc
    assetmanager/assetmanagerwidget.hh
    )

set(ASSETMANAGER_UI
    assetmanager/assetmanagerwidget.ui
    )

source_group(
    "Editor\\assetmanager"
    FILES
    ${ASSETMANAGER_SOURCES}
    ${ASSETMANAGER_UI}
    )

set(ALL_UI
    ${ASSETMANAGER_UI}
    )

qt5_wrap_ui(ALL_UI_SOURCES ${ALL_UI})
qt5_add_resources(RESOURCES_SOURCES resources.qrc)

set(ALL_SOURCES
    ${EDITOR_SOURCES}
    ${ASSETMANAGER_SOURCES}
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

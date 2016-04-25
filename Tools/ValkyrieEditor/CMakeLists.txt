
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

set(EDITOR_SOURCES
    main.cc
    Editor.cc
    Editor.hh
    EditorWindow.cc
    EditorWindow.hh
    SceneView.cc
    SceneView.hh
    )

source_group(
    "Editor"
    FILES
    ${EDITOR_SOURCES}
    )

set(EDITOR_ASSETMANAGER_SOURCES
    AssetManager/AssetManagerWidget.cc
    AssetManager/AssetManagerWidget.hh
    AssetManager/AssetWriter.cc
    AssetManager/AssetWriter.hh
    )

set(EDITOR_ASSETMANAGER_UI
    AssetManager/AssetManagerWidget.ui)

source_group(
  "Editor\\AssetManager"
  FILES
  ${EDITOR_ASSETMANAGER_SOURCES}
  ${EDITOR_ASSETMANAGER_UI}
  )

set(EDITOR_GRAPH_SOURCES
    Graph/Connection.cc
    Graph/Connection.hh
    Graph/Direction.h
    Graph/Node.cc
    Graph/Node.hh
    Graph/Scene.cc
    Graph/Scene.hh
    )

source_group(
    "Editor\\Graph"
    FILES
    ${EDITOR_GRAPH_SOURCES}
    )



set(EDITOR_SHADERGRAPH_SOURCES
    ShaderGraph/Node.cc
    ShaderGraph/Node.hh
    ShaderGraph/NodeSelector.cc
    ShaderGraph/NodeSelector.hh
    ShaderGraph/ShaderGraphView.cc
    ShaderGraph/ShaderGraphView.hh
    ShaderGraph/SGNode.cc
    ShaderGraph/SGNode.hh
    ShaderGraph/SGShaderGraphNode.cc
    ShaderGraph/SGShaderGraphNode.hh
    )

set(EDITOR_SHADERGRAPH_UI
    ShaderGraph/NodeSelector.ui
    ShaderGraph/ShaderGraphView.ui
    )


source_group(
    "Editor\\ShaderGraph"
    FILES
    ${EDITOR_SHADERGRAPH_SOURCES}
    ${EDITOR_SHADERGRAPH_UI}
    )


set(ALL_UI
    ${EDITOR_ASSETMANAGER_UI}
    ${EDITOR_SHADERGRAPH_UI}
    )


qt5_wrap_ui(ALL_UI_SOURCES ${ALL_UI})

add_executable(
    ValkyrieEditor
    ${EDITOR_SOURCES}
    ${EDITOR_ASSETMANAGER_SOURCES}
    ${EDITOR_GRAPH_SOURCES}
    ${EDITOR_SHADERGRAPH_SOURCES}
    ${ALL_UI_SOURCES}
    )

target_link_libraries(ValkyrieEditor 
    Qt5::Core
    Qt5::Widgets
    Qt5::OpenGL
    ValkyrieEngine
    ValkyrieEngine-GraphicsGL4
    ValkyrieEngine-PhysicsBullet
    )


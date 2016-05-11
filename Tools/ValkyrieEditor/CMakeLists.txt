
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

set(EDITOR_SOURCES
    main.cc
    Editor.cc
    Editor.hh
    EditorView.cc
    EditorView.hh
    EditorWindow.cc
    EditorWindow.hh
    SceneView.cc
    SceneView.hh
    Utils.cc
    Utils.hh
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
    AssetManager/EditorIcon.cc
    AssetManager/EditorIcon.hh
    AssetManager/EditorIconLoader.cc
    AssetManager/EditorIconLoader.hh
    AssetManager/FolderItemModel.cc
    AssetManager/FolderItemModel.hh
    AssetManager/Importer.cc
    AssetManager/Importer.hh
    AssetManager/Utils.cc
    AssetManager/Utils.hh
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
    ShaderGraph/EditResourceDefaultDialog.cc
    ShaderGraph/EditResourceDefaultDialog.hh
    ShaderGraph/MetaData.cc
    ShaderGraph/MetaData.hh
    ShaderGraph/MetaDataLoader.cc
    ShaderGraph/MetaDataLoader.hh
    ShaderGraph/Node.cc
    ShaderGraph/Node.hh
    ShaderGraph/NodeSelector.cc
    ShaderGraph/NodeSelector.hh
    ShaderGraph/PreviewWidget.cc
    ShaderGraph/PreviewWidget.hh
    ShaderGraph/ResourcesModel.cc
    ShaderGraph/ResourcesModel.hh
    ShaderGraph/ShaderGraphView.cc
    ShaderGraph/ShaderGraphView.hh
    ShaderGraph/SGNode.cc
    ShaderGraph/SGNode.hh
    ShaderGraph/SGShaderGraphNode.cc
    ShaderGraph/SGShaderGraphNode.hh
    )

set(EDITOR_SHADERGRAPH_UI
    ShaderGraph/EditResourceDefaultDialog.ui
    ShaderGraph/NodeSelector.ui
    ShaderGraph/ShaderGraphView.ui
    )


source_group(
    "Editor\\ShaderGraph"
    FILES
    ${EDITOR_SHADERGRAPH_SOURCES}
    ${EDITOR_SHADERGRAPH_UI}
    )


set(EDITOR_TEXTURE_SOURCES
    Texture/TextureImporter.cc
    Texture/TextureImporter.hh
    )

source_group(
    "Editor\\Texture"
    FILES
    ${EDITOR_TEXTURE_SOURCES}
    )



set(ALL_UI
    ${EDITOR_ASSETMANAGER_UI}
    ${EDITOR_SHADERGRAPH_UI}
    )


qt5_wrap_ui(ALL_UI_SOURCES ${ALL_UI})
qt5_add_resources(RESOURCES_SOURCES resources.qrc)

set(ALL_SOURCES
    ${EDITOR_SOURCES}
    ${EDITOR_ASSETMANAGER_SOURCES}
    ${EDITOR_GRAPH_SOURCES}
    ${EDITOR_SHADERGRAPH_SOURCES}
    ${EDITOR_TEXTURE_SOURCES}
    )

add_executable(
    ValkyrieEditor
    ${ALL_SOURCES}
    ${ALL_UI_SOURCES}
    ${RESOURCES_SOURCES}
    )

target_link_libraries(ValkyrieEditor 
    Qt5::Core
    Qt5::Widgets
    Qt5::OpenGL
    ValkyrieEngine
    ValkyrieEngine-GraphicsGL4
    ValkyrieEngine-PhysicsBullet
    )

VK_INSTALL_MOCCER(ValkyrieEditor NO_PREFIX NO_API ALL_SOURCES)
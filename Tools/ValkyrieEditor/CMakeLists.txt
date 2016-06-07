
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
find_package(assimp)

include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS})

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
    AssetManager/FolderTreeModel.cc
    AssetManager/FolderTreeModel.hh
    AssetManager/NewAssetAction.cc
    AssetManager/NewAssetAction.hh
    AssetManager/NewAssetDialog.cc
    AssetManager/NewAssetDialog.hh
    AssetManager/Importer.cc
    AssetManager/Importer.hh
    AssetManager/Utils.cc
    AssetManager/Utils.hh
    )

set(EDITOR_ASSETMANAGER_UI
    AssetManager/AssetManagerWidget.ui
    AssetManager/NewAssetWidget.ui
    )

source_group(
    "Editor\\AssetManager"
    FILES
    ${EDITOR_ASSETMANAGER_SOURCES}
    ${EDITOR_ASSETMANAGER_UI}
    )

set(EDITOR_COMPONENTS_SOURCES
    Components/ResourceWidget.cc
    Components/ResourceWidget.hh
    )

source_group(
    "Editor\\Components"
    FILES
    ${EDITOR_COMPONENTS_SOURCES}
    )


set(EDITOR_ENTITY_SOURCES
    Entity/EntityView.cc
    Entity/EntityView.hh
    Entity/PreviewWidget.cc
    Entity/PreviewWidget.hh
    )

source_group(
    "Editor\\Entity"
    FILES
    ${EDITOR_ENTITY_SOURCES}
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

set(EDITOR_MATERIAL_INSTANCE_SOURCES
    MaterialInstance/CreateNewMaterialInstance.cc
    MaterialInstance/CreateNewMaterialInstance.hh
    MaterialInstance/MaterialInstanceView.cc
    MaterialInstance/MaterialInstanceView.hh
    MaterialInstance/PreviewWidget.cc
    MaterialInstance/PreviewWidget.hh
    )

set(EDITOR_MATERIAL_INSTANCE_UI
    MaterialInstance/MaterialInstanceView.ui
    )

source_group(
    "Editor\\MaterialInstance"
    FILES
    ${EDITOR_MATERIAL_INSTANCE_SOURCES}
    ${EDITOR_MATERIAL_INSTANCE_UI}
    )

set(EDITOR_MESH_SOURCES
    Mesh/Assimp.cc
    Mesh/Assimp.hh
    Mesh/MeshImporter.cc
    Mesh/MeshImporter.hh
    )

source_group(
    "Editor\\Mesh"
    FILES
    ${EDITOR_MESH_SOURCES}
    )

set(EDITOR_SCENEWIDGET_SOURCES
    SceneWidget/Camera.cc
    SceneWidget/Camera.hh
    SceneWidget/EventListener.hh
    SceneWidget/SceneWidget.cc
    SceneWidget/SceneWidget.hh
    )

source_group(
    "Editor\\SceneWidget"
    FILES
    ${EDITOR_SCENEWIDGET_SOURCES}
    )

set(EDITOR_SHADERGRAPH_SOURCES
    ShaderGraph/CreateNewShaderGraph.cc
    ShaderGraph/CreateNewShaderGraph.hh
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
    ShaderGraph/ShaderEditor.cc
    ShaderGraph/ShaderEditor.hh
    ShaderGraph/ShaderGraphView.cc
    ShaderGraph/ShaderGraphView.hh
    ShaderGraph/SGNode.cc
    ShaderGraph/SGNode.hh
    ShaderGraph/SGShaderGraphNode.cc
    ShaderGraph/SGShaderGraphNode.hh
    )

set(EDITOR_SHADERGRAPH_UI
    ShaderGraph/NodeSelector.ui
    ShaderGraph/ShaderEditor.ui
    ShaderGraph/ShaderGraphView.ui
    )


source_group(
    "Editor\\ShaderGraph"
    FILES
    ${EDITOR_SHADERGRAPH_SOURCES}
    ${EDITOR_SHADERGRAPH_UI}
    )


set(EDITOR_TEXTURE_SOURCES
    Texture/PreviewWidget.cc
    Texture/PreviewWidget.hh
    Texture/TextureImporter.cc
    Texture/TextureImporter.hh
    Texture/TextureView.cc
    Texture/TextureView.hh
    )

set(EDITOR_TEXTURE_UI
    Texture/TextureView.ui
    )


source_group(
    "Editor\\Texture"
    FILES
    ${EDITOR_TEXTURE_SOURCES}
    ${EDITOR_TEXTURE_UI}
    )



set(ALL_UI
    ${EDITOR_ASSETMANAGER_UI}
    ${EDITOR_MATERIAL_INSTANCE_UI}
    ${EDITOR_SHADERGRAPH_UI}
    ${EDITOR_TEXTURE_UI}
    )


qt5_wrap_ui(ALL_UI_SOURCES ${ALL_UI})
qt5_add_resources(RESOURCES_SOURCES resources.qrc)

set(ALL_SOURCES
    ${EDITOR_SOURCES}
    ${EDITOR_ASSETMANAGER_SOURCES}
    ${EDITOR_COMPONENTS_SOURCES}
    ${EDITOR_ENTITY_SOURCES}
    ${EDITOR_GRAPH_SOURCES}
    ${EDITOR_MATERIAL_INSTANCE_SOURCES}
    ${EDITOR_MESH_SOURCES}
    ${EDITOR_SCENEWIDGET_SOURCES}
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
    ${ASSIMP_LIBRARY}
    )

VK_INSTALL_MOCCER(ValkyrieEditor NO_PREFIX NO_API ALL_SOURCES)

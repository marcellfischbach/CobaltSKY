

add_definitions(-DVKE_EXPORT)
add_definitions(-DVK_BUILD_EDITOR)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCE
    vkdefs.hh
    vkdiamond.hh
    vkengine.hh
    vkengine.cc
    vkenums.hh
    vkexport.hh
    vkmodule.cc
    vkmodule.hh
    vktime.cc
    vktime.hh
    vktypes.hh
    )

source_group(
    "ValkyrieEngine"
    FILES
    ${SOURCE}
    )


set(SOURCE_ANIMATION
    Animation/vkskeleton.cc
    Animation/vkskeleton.hh
    )


source_group(
    "ValkyrieEngine\\Animation"
    FILES
    ${SOURCE_ANIMATION}
    )

set(SOURCE_CORE
    Core/ifile.hh
    Core/vkassetinputstream.cc
    Core/vkassetinputstream.hh
    Core/vkassetoutputstream.cc
    Core/vkassetoutputstream.hh
    Core/vkclass.cc
    Core/vkclass.hh
    Core/vkclassregistry.cc
    Core/vkclassregistry.hh
    Core/vkcollection.hh
    Core/vkfileinfo.cc
    Core/vkfileinfo.hh
    Core/vkfilestd.cc
    Core/vkfilestd.hh
    Core/vklibrary.cc
    Core/vklibrary.hh
    Core/vkresourcemanager.cc
    Core/vkresourcemanager.hh
    Core/vksettings.cc
    Core/vksettings.hh
    Core/vkstring.hh
    Core/vkthread.cc
    Core/vkthread.hh
    Core/vkvfs.cc
    Core/vkvfs.hh
    Core/vkxml.cc
    Core/vkxml.hh
    )


source_group(
    "ValkyrieEngine\\Core"
    FILES
    ${SOURCE_CORE}
    )

set(SOURCE_ENTITY
    Entity/Character.cc
    Entity/Character.hh
    Entity/ColliderState.cc
    Entity/ColliderState.hh
    Entity/Entity.cc
    Entity/Entity.hh
    Entity/EntityState.cc
    Entity/EntityState.hh
    Entity/Geometry.cc
    Entity/Geometry.hh
    Entity/JointState.cc
    Entity/JointState.hh
    Entity/LightState.cc
    Entity/LightState.hh
    Entity/MeshState.cc
    Entity/MeshState.hh
    Entity/ParticleState.cc
    Entity/ParticleState.hh
    Entity/RenderState.cc
    Entity/RenderState.hh
    Entity/Scan.hh
    Entity/Scene.cc
    Entity/Scene.hh
    Entity/SpatialState.cc
    Entity/SpatialState.hh
    Entity/Transformation.cc
    Entity/Transformation.hh
    )

source_group(
    "ValkyrieEngine\\Entity"
    FILES
    ${SOURCE_ENTITY}
    )

set(SOURCE_GRAPHICS
    Graphics/BinaryGradient.cc
    Graphics/BinaryGradient.hh
    Graphics/Camera.cc
    Graphics/Camera.hh
    Graphics/IFrameProcessor.hh
    Graphics/IGraphics.hh
    Graphics/IIndexBuffer.hh
    Graphics/Image.cc
    Graphics/Image.hh
    Graphics/IRenderTarget.hh
    Graphics/IShader.hh
    Graphics/ITexture.hh
    Graphics/IVertexBuffer.hh
    Graphics/IVertexDeclaration.hh
    Graphics/Light.cc
    Graphics/Light.hh
    Graphics/Material.cc
    Graphics/Material.hh
    Graphics/Mesh.cc
    Graphics/Mesh.hh
    Graphics/Particle.cc
    Graphics/Particle.hh
    Graphics/PostProcessing.cc
    Graphics/PostProcessing.hh
    Graphics/ShaderID.cc
    Graphics/ShaderID.hh
    )

source_group(
    "ValkyrieEngine\\Graphics"
    FILES
    ${SOURCE_GRAPHICS}
    )
		

set(SOURCE_GRAPHICS_DEFERRED
    Graphics/Deferred/DefaultCollector.cc
    Graphics/Deferred/DefaultCollector.hh
    Graphics/Deferred/DeferredFrameProcessor.cc
    Graphics/Deferred/DeferredFrameProcessor.hh
    Graphics/Deferred/GBuffer.cc
    Graphics/Deferred/GBuffer.hh
    Graphics/Deferred/LightRenderer.cc
    Graphics/Deferred/LightRenderer.hh
    Graphics/Deferred/ParticleRenderer.cc
    Graphics/Deferred/ParticleRenderer.hh
)

source_group(
    "ValkyrieEngine\\Graphics\\Deferred"
    FILES
    ${SOURCE_GRAPHICS_DEFERRED}
    )
		

set(SOURCE_GRAPHICS_SCENE
    Graphics/Scene/CameraNode.cc
    Graphics/Scene/CameraNode.hh
    Graphics/Scene/GeometryNode.cc
    Graphics/Scene/GeometryNode.hh
    Graphics/Scene/GroupNode.cc
    Graphics/Scene/GroupNode.hh
    Graphics/Scene/LightNode.cc
    Graphics/Scene/LightNode.hh
    Graphics/Scene/Node.cc
    Graphics/Scene/Node.hh
    Graphics/Scene/Scan.hh
    Graphics/Scene/SpatialNode.cc
    Graphics/Scene/SpatialNode.hh
    )

source_group(
    "ValkyrieEngine\\Graphics\\Scene"
    FILES
    ${SOURCE_GRAPHICS_SCENE}
    )


set(SOURCE_GRAPHICS_SHADERGRAPH
    Graphics/ShaderGraph/SGNode.cc
    Graphics/ShaderGraph/SGNode.hh
    Graphics/ShaderGraph/SGShaderGraph.cc
    Graphics/ShaderGraph/SGShaderGraph.hh
    )

source_group(
    "ValkyrieEngine\\Graphics\\ShaderGraph"
    FILES
    ${SOURCE_GRAPHICS_SHADERGRAPH}
    )


set(SOURCE_LOADERS
    Loaders/CollisionLoader.cc
    Loaders/CollisionLoader.hh
    Loaders/EntityLoader.cc
    Loaders/EntityLoader.hh
    Loaders/Loaders.cc
    Loaders/Loaders.hh
    Loaders/ImageLoader.cc
    Loaders/ImageLoader.hh
    Loaders/MaterialLoader.cc
    Loaders/MaterialLoader.hh
    Loaders/MaterialInstanceLoader.cc
    Loaders/MaterialInstanceLoader.hh
    Loaders/MeshLoader.cc
    Loaders/MeshLoader.hh
    Loaders/ShaderGraphLoader.cc
    Loaders/ShaderGraphLoader.hh
    Loaders/StaticMeshLoader.cc
    Loaders/StaticMeshLoader.hh
    Loaders/TextureLoader.cc
    Loaders/TextureLoader.hh
    )

source_group(
    "ValkyrieEngine\\Loaders"
    FILES
    ${SOURCE_LOADERS}
    )

set(SOURCE_MATH
    Math/BoundingVolume.cc
    Math/BoundingVolume.hh
    Math/Clipper.cc
    Math/Clipper.hh
    Math/Color.hh
    Math/Hysteresis.hh
    Math/Matrix.hh
    Math/Plane.hh
    Math/RandomRange.hh
    Math/Vector.hh
    )

source_group(
    "ValkyrieEngine\\Math"
    FILES
    ${SOURCE_MATH}
    )

set(SOURCE_PHYSICS
    PHysics/IPhysicsCharacterController.hh
    Physics/IPhysicsCollider.hh
    Physics/IPhysicsJoints.hh
    Physics/IPhysicsScene.hh
    Physics/IPhysicsShape.hh
    Physics/IPhysicsSystem.hh
    Physics/PhysicsShapeContainer.cc
    Physics/PhysicsShapeContainer.hh
    )

source_group(
    "ValkyrieEngine\\Physics"
    FILES
    ${SOURCE_PHYSICS}
    )

set(SOURCE_WINDOW
    Window/IKeyboard.hh
    Window/IMouse.hh
    Window/IWindow.hh
    )

source_group(
    "ValkyrieEngine\\Window"
    FILES
    ${SOURCE_WINDOW}
    )


set(ALL_SOURCES
    ${SOURCE}
    ${SOURCE_ANIMATION}
    ${SOURCE_CORE}
    ${SOURCE_ENTITY}
    ${SOURCE_GRAPHICS}
    ${SOURCE_GRAPHICS_DEFERRED}
    ${SOURCE_GRAPHICS_SCENE}
    ${SOURCE_GRAPHICS_SHADERGRAPH}
    ${SOURCE_LOADERS}
    ${SOURCE_MATH}
    ${SOURCE_PHYSICS}
    ${SOURCE_WINDOW}
    )


add_library(ValkyrieEngine SHARED
    ${ALL_SOURCES}
    )

target_link_libraries(ValkyrieEngine
    TiXml
    ${PNG_LIBRARIES}
    )

VK_INSTALL_MOCCER(ValkyrieEngine Valkyrie VKE_API ALL_SOURCES)


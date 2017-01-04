

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
    "ValkyrieEngine\\animation"
    FILES
    ${SOURCE_ANIMATION}
    )

set(SOURCE_CORE
    core/ifile.hh
    core/vkassetinputstream.cc
    core/vkassetinputstream.hh
    core/vkassetoutputstream.cc
    core/vkassetoutputstream.hh
    core/vkclass.cc
    core/vkclass.hh
    core/vkclassregistry.cc
    core/vkclassregistry.hh
    core/vkcollection.hh
    core/vkfileinfo.cc
    core/vkfileinfo.hh
    core/vkfilestd.cc
    core/vkfilestd.hh
    core/vklibrary.cc
    core/vklibrary.hh
    core/vkresourcelocator.cc
    core/vkresourcelocator.hh
    core/vkresourcemanager.cc
    core/vkresourcemanager.hh
    core/vksettings.cc
    core/vksettings.hh
    core/vkstring.hh
    core/vkthread.cc
    core/vkthread.hh
    core/vkvfs.cc
    core/vkvfs.hh
    core/vkxml.cc
    core/vkxml.hh
    )


source_group(
    "ValkyrieEngine\\core"
    FILES
    ${SOURCE_CORE}
    )

set(SOURCE_CORE_RESOURCE
    core/resource/iassetloader.hh
    core/resource/ifileloader.hh
    core/resource/ixmlloader.hh
    core/resource/vkassetfileloader.hh
    core/resource/vkassetfileloader.cc
    core/resource/vkassetxmlloader.hh
    core/resource/vkassetxmlloader.cc
    core/resource/vkbasexmlloader.hh
    core/resource/vkbasexmlloader.cc
    core/resource/vkxmlfileloader.hh
    core/resource/vkxmlfileloader.cc
    )

source_group(
    "ValkyrieEngine\\core\\resource"
    FILES
    ${SOURCE_CORE_RESOURCE}
    )


set(SOURCE_ENTITY
    entity/ientityscan.hh
    entity/iparticleemitter.hh
    entity/iparticlestepper.hh
    entity/itransformationcallback.hh
    entity/vkbasecolliderstate.cc
    entity/vkbasecolliderstate.hh
    entity/vkcharacterentity.cc
    entity/vkcharacterentity.hh
    entity/vkcolliderstate.cc
    entity/vkcolliderstate.hh
    entity/vkdefaultparticleemitter.cc
    entity/vkdefaultparticleemitter.hh
    entity/vkdefaultparticlestepper.cc
    entity/vkdefaultparticlestepper.hh
    entity/vkdynamiccolliderstate.cc
    entity/vkdynamiccolliderstate.hh
    entity/vkentity.cc
    entity/vkentity.hh
    entity/vkentityscene.cc
    entity/vkentityscene.hh
    entity/vkentitystate.cc
    entity/vkentitystate.hh
    entity/vkgeometrydata.cc
    entity/vkgeometrydata.hh
    entity/vkgeometrymesh.cc
    entity/vkgeometrymesh.hh
    entity/vkhingejointstate.cc
    entity/vkhingejointstate.hh
    entity/vkjointstate.cc
    entity/vkjointstate.hh
    entity/vklightstate.cc
    entity/vklightstate.hh
    entity/vkparticlestate.cc
    entity/vkparticlestate.hh
    entity/vkrenderstate.cc
    entity/vkrenderstate.hh
    entity/vkspatialstate.cc
    entity/vkspatialstate.hh
    entity/vkstaticcolliderstate.cc
    entity/vkstaticcolliderstate.hh
    entity/vkstaticmeshstate.cc
    entity/vkstaticmeshstate.hh
    entity/vktransformation.cc
    entity/vktransformation.hh
    )

source_group(
    "ValkyrieEngine\\entity"
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
    Graphics/Deferred/vkdefaultcollector.cc
    Graphics/Deferred/vkdefaultcollector.hh
    Graphics/Deferred/vkdeferredframeprocessor.cc
    Graphics/Deferred/vkdeferredframeprocessor.hh
    Graphics/Deferred/vkdirectionallightrenderer.cc
    Graphics/Deferred/vkdirectionallightrenderer.hh
    Graphics/Deferred/vkgbuffer.cc
    Graphics/Deferred/vkgbuffer.hh
    Graphics/Deferred/vklightrenderer.cc
    Graphics/Deferred/vklightrenderer.hh
    Graphics/Deferred/vkparticlerenderer.cc
    Graphics/Deferred/vkparticlerenderer.hh
    Graphics/Deferred/vkpointlightrenderer.cc
    Graphics/Deferred/vkpointlightrenderer.hh
)

source_group(
    "ValkyrieEngine\\Graphics\\Deferred"
    FILES
    ${SOURCE_GRAPHICS_DEFERRED}
    )
		

set(SOURCE_GRAPHICS_SCENE
    Graphics/Scene/iscancallback.hh
    Graphics/Scene/vkcameranode.cc
    Graphics/Scene/vkcameranode.hh
    Graphics/Scene/vkgeometrynode.cc
    Graphics/Scene/vkgeometrynode.hh
    Graphics/Scene/vkgroupnode.cc
    Graphics/Scene/vkgroupnode.hh
    Graphics/Scene/vklightnode.cc
    Graphics/Scene/vklightnode.hh
    Graphics/Scene/vknode.cc
    Graphics/Scene/vknode.hh
    Graphics/Scene/vkspatialnode.cc
    Graphics/Scene/vkspatialnode.hh
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
    ${SOURCE_CORE_RESOURCE}
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


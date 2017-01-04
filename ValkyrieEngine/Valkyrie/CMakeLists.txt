

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
    Graphics/deferred/vkdefaultcollector.cc
    Graphics/deferred/vkdefaultcollector.hh
    Graphics/deferred/vkdeferredframeprocessor.cc
    Graphics/deferred/vkdeferredframeprocessor.hh
    Graphics/deferred/vkdirectionallightrenderer.cc
    Graphics/deferred/vkdirectionallightrenderer.hh
    Graphics/deferred/vkgbuffer.cc
    Graphics/deferred/vkgbuffer.hh
    Graphics/deferred/vklightrenderer.cc
    Graphics/deferred/vklightrenderer.hh
    Graphics/deferred/vkparticlerenderer.cc
    Graphics/deferred/vkparticlerenderer.hh
    Graphics/deferred/vkpointlightrenderer.cc
    Graphics/deferred/vkpointlightrenderer.hh
)

source_group(
    "ValkyrieEngine\\Graphics\\deferred"
    FILES
    ${SOURCE_GRAPHICS_DEFERRED}
    )
		

set(SOURCE_GRAPHICS_SCENE
    Graphics/scene/iscancallback.hh
    Graphics/scene/vkcameranode.cc
    Graphics/scene/vkcameranode.hh
    Graphics/scene/vkgeometrynode.cc
    Graphics/scene/vkgeometrynode.hh
    Graphics/scene/vkgroupnode.cc
    Graphics/scene/vkgroupnode.hh
    Graphics/scene/vklightnode.cc
    Graphics/scene/vklightnode.hh
    Graphics/scene/vknode.cc
    Graphics/scene/vknode.hh
    Graphics/scene/vkspatialnode.cc
    Graphics/scene/vkspatialnode.hh
    )

source_group(
    "ValkyrieEngine\\Graphics\\scene"
    FILES
    ${SOURCE_GRAPHICS_SCENE}
    )


set(SOURCE_GRAPHICS_SHADERGRAPH
    Graphics/ShaderGraph/vksgadd.cc
    Graphics/ShaderGraph/vksgadd.hh
    Graphics/ShaderGraph/vksgconstfloat1.cc
    Graphics/ShaderGraph/vksgconstfloat1.hh
    Graphics/ShaderGraph/vksgconstfloat2.cc
    Graphics/ShaderGraph/vksgconstfloat2.hh
    Graphics/ShaderGraph/vksgconstfloat3.cc
    Graphics/ShaderGraph/vksgconstfloat3.hh
    Graphics/ShaderGraph/vksgconstfloat4.cc
    Graphics/ShaderGraph/vksgconstfloat4.hh
    Graphics/ShaderGraph/vksgcross.cc
    Graphics/ShaderGraph/vksgcross.hh
    Graphics/ShaderGraph/vksgdefaulttexturecoordinate.cc
    Graphics/ShaderGraph/vksgdefaulttexturecoordinate.hh
    Graphics/ShaderGraph/vksgdiv.cc
    Graphics/ShaderGraph/vksgdiv.hh
    Graphics/ShaderGraph/vksgdot.cc
    Graphics/ShaderGraph/vksgdot.hh
    Graphics/ShaderGraph/vksgenums.hh
    Graphics/ShaderGraph/vksgfloat2.cc
    Graphics/ShaderGraph/vksgfloat2.hh
    Graphics/ShaderGraph/vksgfloat3.cc
    Graphics/ShaderGraph/vksgfloat3.hh
    Graphics/ShaderGraph/vksgfloat4.cc
    Graphics/ShaderGraph/vksgfloat4.hh
    Graphics/ShaderGraph/vksginput.cc
    Graphics/ShaderGraph/vksginput.hh
    Graphics/ShaderGraph/vksglerp.cc
    Graphics/ShaderGraph/vksglerp.hh
    Graphics/ShaderGraph/vksgmul.cc
    Graphics/ShaderGraph/vksgmul.hh
    Graphics/ShaderGraph/vksgnode.cc
    Graphics/ShaderGraph/vksgnode.hh
    Graphics/ShaderGraph/vksgnodes.cc
    Graphics/ShaderGraph/vksgnodes.hh
    Graphics/ShaderGraph/vksgoutput.cc
    Graphics/ShaderGraph/vksgoutput.hh
    Graphics/ShaderGraph/vksgresourcenode.cc
    Graphics/ShaderGraph/vksgresourcenode.hh
    Graphics/ShaderGraph/vksgshadergraph.cc
    Graphics/ShaderGraph/vksgshadergraph.hh
    Graphics/ShaderGraph/vksgsplitfloat2.cc
    Graphics/ShaderGraph/vksgsplitfloat2.hh
    Graphics/ShaderGraph/vksgsplitfloat3.cc
    Graphics/ShaderGraph/vksgsplitfloat3.hh
    Graphics/ShaderGraph/vksgsplitfloat4.cc
    Graphics/ShaderGraph/vksgsplitfloat4.hh
    Graphics/ShaderGraph/vksgsub.cc
    Graphics/ShaderGraph/vksgsub.hh
    Graphics/ShaderGraph/vksgtexture2d.cc
    Graphics/ShaderGraph/vksgtexture2d.hh
    Graphics/ShaderGraph/vksgvarfloat1.cc
    Graphics/ShaderGraph/vksgvarfloat1.hh
    Graphics/ShaderGraph/vksgvarfloat2.cc
    Graphics/ShaderGraph/vksgvarfloat2.hh
    Graphics/ShaderGraph/vksgvarfloat3.cc
    Graphics/ShaderGraph/vksgvarfloat3.hh
    Graphics/ShaderGraph/vksgvarfloat4.cc
    Graphics/ShaderGraph/vksgvarfloat4.hh
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


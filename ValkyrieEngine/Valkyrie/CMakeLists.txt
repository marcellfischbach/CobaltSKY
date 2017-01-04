

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
    Graphics/shadergraph/vksgadd.cc
    Graphics/shadergraph/vksgadd.hh
    Graphics/shadergraph/vksgconstfloat1.cc
    Graphics/shadergraph/vksgconstfloat1.hh
    Graphics/shadergraph/vksgconstfloat2.cc
    Graphics/shadergraph/vksgconstfloat2.hh
    Graphics/shadergraph/vksgconstfloat3.cc
    Graphics/shadergraph/vksgconstfloat3.hh
    Graphics/shadergraph/vksgconstfloat4.cc
    Graphics/shadergraph/vksgconstfloat4.hh
    Graphics/shadergraph/vksgcross.cc
    Graphics/shadergraph/vksgcross.hh
    Graphics/shadergraph/vksgdefaulttexturecoordinate.cc
    Graphics/shadergraph/vksgdefaulttexturecoordinate.hh
    Graphics/shadergraph/vksgdiv.cc
    Graphics/shadergraph/vksgdiv.hh
    Graphics/shadergraph/vksgdot.cc
    Graphics/shadergraph/vksgdot.hh
    Graphics/shadergraph/vksgenums.hh
    Graphics/shadergraph/vksgfloat2.cc
    Graphics/shadergraph/vksgfloat2.hh
    Graphics/shadergraph/vksgfloat3.cc
    Graphics/shadergraph/vksgfloat3.hh
    Graphics/shadergraph/vksgfloat4.cc
    Graphics/shadergraph/vksgfloat4.hh
    Graphics/shadergraph/vksginput.cc
    Graphics/shadergraph/vksginput.hh
    Graphics/shadergraph/vksglerp.cc
    Graphics/shadergraph/vksglerp.hh
    Graphics/shadergraph/vksgmul.cc
    Graphics/shadergraph/vksgmul.hh
    Graphics/shadergraph/vksgnode.cc
    Graphics/shadergraph/vksgnode.hh
    Graphics/shadergraph/vksgnodes.cc
    Graphics/shadergraph/vksgnodes.hh
    Graphics/shadergraph/vksgoutput.cc
    Graphics/shadergraph/vksgoutput.hh
    Graphics/shadergraph/vksgresourcenode.cc
    Graphics/shadergraph/vksgresourcenode.hh
    Graphics/shadergraph/vksgshadergraph.cc
    Graphics/shadergraph/vksgshadergraph.hh
    Graphics/shadergraph/vksgsplitfloat2.cc
    Graphics/shadergraph/vksgsplitfloat2.hh
    Graphics/shadergraph/vksgsplitfloat3.cc
    Graphics/shadergraph/vksgsplitfloat3.hh
    Graphics/shadergraph/vksgsplitfloat4.cc
    Graphics/shadergraph/vksgsplitfloat4.hh
    Graphics/shadergraph/vksgsub.cc
    Graphics/shadergraph/vksgsub.hh
    Graphics/shadergraph/vksgtexture2d.cc
    Graphics/shadergraph/vksgtexture2d.hh
    Graphics/shadergraph/vksgvarfloat1.cc
    Graphics/shadergraph/vksgvarfloat1.hh
    Graphics/shadergraph/vksgvarfloat2.cc
    Graphics/shadergraph/vksgvarfloat2.hh
    Graphics/shadergraph/vksgvarfloat3.cc
    Graphics/shadergraph/vksgvarfloat3.hh
    Graphics/shadergraph/vksgvarfloat4.cc
    Graphics/shadergraph/vksgvarfloat4.hh
    )


source_group(
    "ValkyrieEngine\\Graphics\\shadergraph"
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


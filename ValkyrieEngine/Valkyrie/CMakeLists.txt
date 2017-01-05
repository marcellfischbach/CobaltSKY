

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
    entity/vkskinnedmeshstate.cc
    entity/vkskinnedmeshstate.hh
    entity/vktransformation.cc
    entity/vktransformation.hh
    )

source_group(
    "ValkyrieEngine\\entity"
    FILES
    ${SOURCE_ENTITY}
    )

set(SOURCE_GRAPHICS
    graphics/iframeprocessor.hh
    graphics/igraphics.hh
    graphics/iindexbuffer.hh
    graphics/irendertarget.hh
    graphics/ishader.hh
    graphics/ishaderattribute.hh
    graphics/ishaderstream.hh
		graphics/isampler.hh
    graphics/itexture.hh
    graphics/itexture2d.hh
    graphics/itexture2darray.hh
    graphics/itexturecube.hh
    graphics/ivertexbuffer.hh
    graphics/ivertexdeclaration.hh
    graphics/vkbinarygradient.cc
    graphics/vkbinarygradient.hh
    graphics/vkcamera.cc
    graphics/vkcamera.hh
    graphics/vkdirectionallight.cc
    graphics/vkdirectionallight.hh
    graphics/vkgenericshaderpostprocess.cc
    graphics/vkgenericshaderpostprocess.hh
    graphics/vkimage.cc
    graphics/vkimage.hh
    graphics/vklight.cc
    graphics/vklight.hh
    graphics/vkmaterial.cc
    graphics/vkmaterial.hh
    graphics/vkmaterialinstance.cc
    graphics/vkmaterialinstance.hh
    graphics/vkmultimaterial.cc
    graphics/vkmultimaterial.hh
    graphics/vkmesh.cc
    graphics/vkmesh.hh
    graphics/vkparticle.cc
    graphics/vkparticle.hh
    graphics/vkpointlight.cc
    graphics/vkpointlight.hh
    graphics/vkpostprocess.cc
    graphics/vkpostprocess.hh
    graphics/vkpostprocessoutput.hh
    graphics/vkpostprocessor.cc
    graphics/vkpostprocessor.hh
    graphics/vkskinnedmesh.cc
    graphics/vkskinnedmesh.hh
    graphics/vksubmesh.cc
    graphics/vksubmesh.hh
    graphics/vkshaderattributeid.cc
    graphics/vkshaderattributeid.hh
    graphics/vkshaderstreamid.cc
    graphics/vkshaderstreamid.hh
    )

source_group(
    "ValkyrieEngine\\graphics"
    FILES
    ${SOURCE_GRAPHICS}
    )
		

set(SOURCE_GRAPHICS_DEFERRED
    graphics/deferred/vkdefaultcollector.cc
    graphics/deferred/vkdefaultcollector.hh
    graphics/deferred/vkdeferredframeprocessor.cc
    graphics/deferred/vkdeferredframeprocessor.hh
    graphics/deferred/vkdirectionallightrenderer.cc
    graphics/deferred/vkdirectionallightrenderer.hh
    graphics/deferred/vkgbuffer.cc
    graphics/deferred/vkgbuffer.hh
    graphics/deferred/vklightrenderer.cc
    graphics/deferred/vklightrenderer.hh
    graphics/deferred/vkparticlerenderer.cc
    graphics/deferred/vkparticlerenderer.hh
    graphics/deferred/vkpointlightrenderer.cc
    graphics/deferred/vkpointlightrenderer.hh
)

source_group(
    "ValkyrieEngine\\graphics\\deferred"
    FILES
    ${SOURCE_GRAPHICS_DEFERRED}
    )
		

set(SOURCE_GRAPHICS_SCENE
    graphics/scene/iscancallback.hh
    graphics/scene/vkcameranode.cc
    graphics/scene/vkcameranode.hh
    graphics/scene/vkgeometrynode.cc
    graphics/scene/vkgeometrynode.hh
    graphics/scene/vkgroupnode.cc
    graphics/scene/vkgroupnode.hh
    graphics/scene/vklightnode.cc
    graphics/scene/vklightnode.hh
    graphics/scene/vknode.cc
    graphics/scene/vknode.hh
    graphics/scene/vkspatialnode.cc
    graphics/scene/vkspatialnode.hh
    )

source_group(
    "ValkyrieEngine\\graphics\\scene"
    FILES
    ${SOURCE_GRAPHICS_SCENE}
    )


set(SOURCE_GRAPHICS_SHADERGRAPH
    graphics/shadergraph/vksgadd.cc
    graphics/shadergraph/vksgadd.hh
    graphics/shadergraph/vksgconstfloat1.cc
    graphics/shadergraph/vksgconstfloat1.hh
    graphics/shadergraph/vksgconstfloat2.cc
    graphics/shadergraph/vksgconstfloat2.hh
    graphics/shadergraph/vksgconstfloat3.cc
    graphics/shadergraph/vksgconstfloat3.hh
    graphics/shadergraph/vksgconstfloat4.cc
    graphics/shadergraph/vksgconstfloat4.hh
    graphics/shadergraph/vksgcross.cc
    graphics/shadergraph/vksgcross.hh
    graphics/shadergraph/vksgdefaulttexturecoordinate.cc
    graphics/shadergraph/vksgdefaulttexturecoordinate.hh
    graphics/shadergraph/vksgdiv.cc
    graphics/shadergraph/vksgdiv.hh
    graphics/shadergraph/vksgdot.cc
    graphics/shadergraph/vksgdot.hh
    graphics/shadergraph/vksgenums.hh
    graphics/shadergraph/vksgfloat2.cc
    graphics/shadergraph/vksgfloat2.hh
    graphics/shadergraph/vksgfloat3.cc
    graphics/shadergraph/vksgfloat3.hh
    graphics/shadergraph/vksgfloat4.cc
    graphics/shadergraph/vksgfloat4.hh
    graphics/shadergraph/vksginput.cc
    graphics/shadergraph/vksginput.hh
    graphics/shadergraph/vksglerp.cc
    graphics/shadergraph/vksglerp.hh
    graphics/shadergraph/vksgmul.cc
    graphics/shadergraph/vksgmul.hh
    graphics/shadergraph/vksgnode.cc
    graphics/shadergraph/vksgnode.hh
    graphics/shadergraph/vksgnodes.cc
    graphics/shadergraph/vksgnodes.hh
    graphics/shadergraph/vksgoutput.cc
    graphics/shadergraph/vksgoutput.hh
    graphics/shadergraph/vksgresourcenode.cc
    graphics/shadergraph/vksgresourcenode.hh
    graphics/shadergraph/vksgshadergraph.cc
    graphics/shadergraph/vksgshadergraph.hh
    graphics/shadergraph/vksgsplitfloat2.cc
    graphics/shadergraph/vksgsplitfloat2.hh
    graphics/shadergraph/vksgsplitfloat3.cc
    graphics/shadergraph/vksgsplitfloat3.hh
    graphics/shadergraph/vksgsplitfloat4.cc
    graphics/shadergraph/vksgsplitfloat4.hh
    graphics/shadergraph/vksgsub.cc
    graphics/shadergraph/vksgsub.hh
    graphics/shadergraph/vksgtexture2d.cc
    graphics/shadergraph/vksgtexture2d.hh
    graphics/shadergraph/vksgvarfloat1.cc
    graphics/shadergraph/vksgvarfloat1.hh
    graphics/shadergraph/vksgvarfloat2.cc
    graphics/shadergraph/vksgvarfloat2.hh
    graphics/shadergraph/vksgvarfloat3.cc
    graphics/shadergraph/vksgvarfloat3.hh
    graphics/shadergraph/vksgvarfloat4.cc
    graphics/shadergraph/vksgvarfloat4.hh
    )


source_group(
    "ValkyrieEngine\\graphics\\shadergraph"
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
    Physics/iphysicsbasecollider.hh
    Physics/iphysicscharactercontroller.hh
    Physics/iphysicscapsulecharactercontroller.hh
    Physics/iphysicscollider.hh
    Physics/iphysicsdynamiccollider.hh
    Physics/iphysicsjoint.hh
    Physics/iphysicshingejoint.hh
    Physics/iphysicsscene.hh
    Physics/iphysicsshape.hh
    Physics/iphysicsstaticcollider.hh
    Physics/iphysicssystem.hh
    Physics/iphysicsstaticcollider.hh
    Physics/iphysicstriggercollider.hh
    Physics/vkphysicsshapecontainer.cc
    Physics/vkphysicsshapecontainer.hh
    )

source_group(
    "ValkyrieEngine\\physics"
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


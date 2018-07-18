

add_definitions(-DCSE_EXPORT)
add_definitions(-DCS_BUILD_EDITOR)
add_definitions(-D_USE_MATH_DEFINES)

set(SOURCE
    csdefs.hh
    csdiamond.hh
    csengine.hh
    csengine.cc
    csenums.hh
    csexport.hh
    csmodule.cc
    csmodule.hh
    cstime.cc
    cstime.hh
    cstypes.hh
    )

source_group(
    "cobalt"
    FILES
    ${SOURCE}
    )


set(SOURCE_ANIMATION
    animation/csskeleton.cc
    animation/csskeleton.hh
    )


source_group(
    "cobalt\\animation"
    FILES
    ${SOURCE_ANIMATION}
    )

set(SOURCE_CORE
    core/ifile.hh
    core/csassetinputstream.cc
    core/csassetinputstream.hh
    core/csassetoutputstream.cc
    core/csassetoutputstream.hh
    core/csclass.cc
    core/csclass.hh
    core/csclassregistry.cc
    core/csclassregistry.hh
    core/cscollection.hh
    core/csevent.cc
    core/csevent.hh
    core/cseventbus.cc
    core/cseventbus.hh
    core/csfileinfo.cc
    core/csfileinfo.hh
    core/csfilestd.cc
    core/csfilestd.hh
	core/csfwriter.cc
	core/csfwriter.hh
    core/csini.cc
    core/csini.hh
    core/cslibrary.cc
    core/cslibrary.hh
    core/cslogger.cc
    core/cslogger.hh
	core/csreflectionhelper.cc
	core/csreflectionhelper.hh
    core/csresourcelocator.cc
    core/csresourcelocator.hh
    core/csresourcemanager.cc
    core/csresourcemanager.hh
    core/cssettings.cc
    core/cssettings.hh
    core/csstring.hh
    core/csthread.cc
    core/csthread.hh
    core/csvfs.cc
    core/csvfs.hh
    core/csxml.cc
    core/csxml.hh
    )


source_group(
    "cobalt\\core"
    FILES
    ${SOURCE_CORE}
    )

	
set(SOURCE_CORE_PROPERTY
    core/property/csgenericpropertysetter.hh
    core/property/cspropertysetter.hh
    core/property/cspropertysetter.cc
    core/property/csresourcepropertysetter.hh
    core/property/csresourcepropertysetter.cc
    )

source_group(
    "cobalt\\core\\property"
    FILES
    ${SOURCE_CORE_PROPERTY}
    )



set(SOURCE_CORE_RESOURCE
    core/resource/iassetloader.hh
    core/resource/icsfloader.hh
    core/resource/ifileloader.hh
    core/resource/ixmlloader.hh
    core/resource/csassetcsfloader.hh
    core/resource/csassetcsfloader.cc
    core/resource/csassetxmlloader.hh
    core/resource/csassetxmlloader.cc
    core/resource/csbasecsfloader.hh
    core/resource/csbasecsfloader.cc
    core/resource/csbasexmlloader.hh
    core/resource/csbasexmlloader.cc
    core/resource/cscsffileloader.hh
    core/resource/cscsffileloader.cc
    core/resource/csxmlfileloader.hh
    core/resource/csxmlfileloader.cc
    )

source_group(
    "cobalt\\core\\resource"
    FILES
    ${SOURCE_CORE_RESOURCE}
    )




set(SOURCE_ENTITY
    entity/ientityscan.hh
    entity/iparticleemitter.hh
    entity/iparticlestepper.hh
    entity/itransformationcallback.hh
    entity/csbasecolliderstate.cc
    entity/csbasecolliderstate.hh
    entity/csblueprint.cc
    entity/csblueprint.hh
    entity/cscharacterentity.cc
    entity/cscharacterentity.hh
    entity/cscolliderstate.cc
    entity/cscolliderstate.hh
    entity/csdefaultparticleemitter.cc
    entity/csdefaultparticleemitter.hh
    entity/csdefaultparticlestepper.cc
    entity/csdefaultparticlestepper.hh
    entity/csdynamiccolliderstate.cc
    entity/csdynamiccolliderstate.hh
    entity/csentity.cc
    entity/csentity.hh
    entity/csentityscene.cc
    entity/csentityscene.hh
    entity/csentitystate.cc
    entity/csentitystate.hh
    entity/csgeometrydata.cc
    entity/csgeometrydata.hh
    entity/csgeometrymesh.cc
    entity/csgeometrymesh.hh
    entity/cshingejointstate.cc
    entity/cshingejointstate.hh
    entity/csjointstate.cc
    entity/csjointstate.hh
    entity/cslightstate.cc
    entity/cslightstate.hh
    entity/csparticlestate.cc
    entity/csparticlestate.hh
    entity/csrenderstate.cc
    entity/csrenderstate.hh
    entity/csspatialstate.cc
    entity/csspatialstate.hh
    entity/csstaticcolliderstate.cc
    entity/csstaticcolliderstate.hh
    entity/csstaticmeshstate.cc
    entity/csstaticmeshstate.hh
    entity/csskinnedmeshstate.cc
    entity/csskinnedmeshstate.hh
    entity/cstransformation.cc
    entity/cstransformation.hh
    entity/cstransformstate.cc
    entity/cstransformstate.hh
    )

source_group(
    "cobalt\\entity"
    FILES
    ${SOURCE_ENTITY}
    )

set(SOURCE_ENTITY_BLUEPRINT
    entity/blueprint/csblueprint.cc
    entity/blueprint/csblueprint.hh
    entity/blueprint/csbpentity.cc
    entity/blueprint/csbpentity.hh
    entity/blueprint/csbpentitystate.cc
    entity/blueprint/csbpentitystate.hh
    )

source_group(
    "cobalt\\entity\\blueprint"
    FILES
    ${SOURCE_ENTITY_BLUEPRINT}
    )

set(SOURCE_GRAPHICS
    graphics/iframeprocessor.hh
    graphics/igraphics.hh
    graphics/iindexbuffer.hh
    graphics/imesh.hh
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
    graphics/csbinarygradient.cc
    graphics/csbinarygradient.hh
    graphics/cscamera.cc
    graphics/cscamera.hh
    graphics/csdirectionallight.cc
    graphics/csdirectionallight.hh
    graphics/csgenericshaderpostprocess.cc
    graphics/csgenericshaderpostprocess.hh
    graphics/csimage.cc
    graphics/csimage.hh
    graphics/cslight.cc
    graphics/cslight.hh
    graphics/csmaterialdef.cc
    graphics/csmaterialdef.hh
    graphics/csmaterial.cc
    graphics/csmaterial.hh
    graphics/csmultimaterial.cc
    graphics/csmultimaterial.hh
    graphics/csmesh.cc
    graphics/csmesh.hh
    graphics/csparticle.cc
    graphics/csparticle.hh
    graphics/cspointlight.cc
    graphics/cspointlight.hh
    graphics/cspostprocess.cc
    graphics/cspostprocess.hh
    graphics/cspostprocessoutput.hh
    graphics/cspostprocessor.cc
    graphics/cspostprocessor.hh
    graphics/csskinnedmesh.cc
    graphics/csskinnedmesh.hh
    graphics/cssubmesh.cc
    graphics/cssubmesh.hh
    graphics/csshaderattributeid.cc
    graphics/csshaderattributeid.hh
    graphics/csshaderstreamid.cc
    graphics/csshaderstreamid.hh
    graphics/csterrainmesh.cc
    graphics/csterrainmesh.hh
    )

source_group(
    "cobalt\\graphics"
    FILES
    ${SOURCE_GRAPHICS}
    )
		


set(SOURCE_GRAPHICS_SCENE
    graphics/scene/iscancallback.hh
    graphics/scene/cscameranode.cc
    graphics/scene/cscameranode.hh
    graphics/scene/csgeometrynode.cc
    graphics/scene/csgeometrynode.hh
    graphics/scene/csgroupnode.cc
    graphics/scene/csgroupnode.hh
    graphics/scene/cslightnode.cc
    graphics/scene/cslightnode.hh
    graphics/scene/csnode.cc
    graphics/scene/csnode.hh
    graphics/scene/csspatialnode.cc
    graphics/scene/csspatialnode.hh
    )

source_group(
    "cobalt\\graphics\\scene"
    FILES
    ${SOURCE_GRAPHICS_SCENE}
    )


set(SOURCE_GRAPHICS_SHADERGRAPH
    graphics/shadergraph/cssgadd.cc
    graphics/shadergraph/cssgadd.hh
    graphics/shadergraph/cssgconstfloat1.cc
    graphics/shadergraph/cssgconstfloat1.hh
    graphics/shadergraph/cssgconstfloat2.cc
    graphics/shadergraph/cssgconstfloat2.hh
    graphics/shadergraph/cssgconstfloat3.cc
    graphics/shadergraph/cssgconstfloat3.hh
    graphics/shadergraph/cssgconstfloat4.cc
    graphics/shadergraph/cssgconstfloat4.hh
    graphics/shadergraph/cssgcross.cc
    graphics/shadergraph/cssgcross.hh
    graphics/shadergraph/cssgdefaultconsolelogger.cc
    graphics/shadergraph/cssgdefaultconsolelogger.hh
    graphics/shadergraph/cssgdefaulttexturecoordinate.cc
    graphics/shadergraph/cssgdefaulttexturecoordinate.hh
    graphics/shadergraph/cssgdiv.cc
    graphics/shadergraph/cssgdiv.hh
    graphics/shadergraph/cssgdot.cc
    graphics/shadergraph/cssgdot.hh
    graphics/shadergraph/cssgenums.hh
    graphics/shadergraph/cssgfloat2.cc
    graphics/shadergraph/cssgfloat2.hh
    graphics/shadergraph/cssgfloat3.cc
    graphics/shadergraph/cssgfloat3.hh
    graphics/shadergraph/cssgfloat4.cc
    graphics/shadergraph/cssgfloat4.hh
    graphics/shadergraph/cssginput.cc
    graphics/shadergraph/cssginput.hh
    graphics/shadergraph/cssglerp.cc
    graphics/shadergraph/cssglerp.hh
    graphics/shadergraph/cssgmul.cc
    graphics/shadergraph/cssgmul.hh
    graphics/shadergraph/cssgnode.cc
    graphics/shadergraph/cssgnode.hh
    graphics/shadergraph/cssgnodes.cc
    graphics/shadergraph/cssgnodes.hh
    graphics/shadergraph/cssgnormalize.cc
    graphics/shadergraph/cssgnormalize.hh
    graphics/shadergraph/cssgoutput.cc
    graphics/shadergraph/cssgoutput.hh
    graphics/shadergraph/cssgresourcenode.cc
    graphics/shadergraph/cssgresourcenode.hh
    graphics/shadergraph/cssgshadergraph.cc
    graphics/shadergraph/cssgshadergraph.hh
    graphics/shadergraph/cssgsplitfloat2.cc
    graphics/shadergraph/cssgsplitfloat2.hh
    graphics/shadergraph/cssgsplitfloat3.cc
    graphics/shadergraph/cssgsplitfloat3.hh
    graphics/shadergraph/cssgsplitfloat4.cc
    graphics/shadergraph/cssgsplitfloat4.hh
    graphics/shadergraph/cssgsub.cc
    graphics/shadergraph/cssgsub.hh
    graphics/shadergraph/cssgtexture2d.cc
    graphics/shadergraph/cssgtexture2d.hh
    graphics/shadergraph/cssgvarfloat1.cc
    graphics/shadergraph/cssgvarfloat1.hh
    graphics/shadergraph/cssgvarfloat2.cc
    graphics/shadergraph/cssgvarfloat2.hh
    graphics/shadergraph/cssgvarfloat3.cc
    graphics/shadergraph/cssgvarfloat3.hh
    graphics/shadergraph/cssgvarfloat4.cc
    graphics/shadergraph/cssgvarfloat4.hh
    )


source_group(
    "cobalt\\graphics\\shadergraph"
    FILES
    ${SOURCE_GRAPHICS_SHADERGRAPH}
    )


set(SOURCE_LOADERS
	loaders/csblueprintcsfloader.cc
	loaders/csblueprintcsfloader.hh    
	loaders/cscollisionassetcsfloader.cc
    loaders/cscollisionassetcsfloader.hh
	loaders/csentitycsfloader.cc
	loaders/csentitycsfloader.hh
	loaders/csentitystatecsfloader.cc
	loaders/csentitystatecsfloader.hh
    loaders/csloaders.cc
    loaders/csloaders.hh
    loaders/csmaterialassetcsfloader.cc
    loaders/csmaterialassetcsfloader.hh
    loaders/csmaterialdefassetcsfloader.cc
    loaders/csmaterialdefassetcsfloader.hh
    loaders/csmeshassetcsfloader.cc
    loaders/csmeshassetcsfloader.hh
    loaders/cspngimageassetloader.cc
    loaders/cspngimageassetloader.hh
    loaders/cspngimagefileloader.cc
    loaders/cspngimagefileloader.hh
	loaders/cspropertysettercsfloader.cc
	loaders/cspropertysettercsfloader.hh
    loaders/cssamplerassetcsfloader.cc
    loaders/cssamplerassetcsfloader.hh
    loaders/csshadergraphassetcsfloader.cc
    loaders/csshadergraphassetcsfloader.hh
    loaders/csskinnedmeshassetcsfloader.cc
    loaders/csskinnedmeshassetcsfloader.hh
    loaders/csstaticmeshloader.cc
    loaders/csstaticmeshloader.hh
    loaders/csstaticmeshassetloader.cc
    loaders/csstaticmeshassetloader.hh
    loaders/cssubmeshassetloader.cc
    loaders/cssubmeshassetloader.hh
    loaders/cstextureassetcsfloader.cc
    loaders/cstextureassetcsfloader.hh
    )

source_group(
    "cobalt\\loaders"
    FILES
    ${SOURCE_LOADERS}
    )




set(SOURCE_MATH
    math/csboundingbox.cc
    math/csboundingbox.hh
    math/csclipper.cc
    math/csclipper.hh
    math/cscolor4f.hh
    math/cshysteresis.hh
    math/csmatrix.hh
    math/csmatrix3f.hh
    math/csmatrix4f.hh
    math/csplane.hh
    math/csplaneclipper.cc
    math/csplaneclipper.hh
    math/csrandomrange.hh
    math/csvector.hh
    math/csvector2f.hh
    math/csvector3f.hh
    math/csvector4f.hh
    )

source_group(
    "cobalt\\math"
    FILES
    ${SOURCE_MATH}
    )

set(SOURCE_PHYSICS
    physics/csphysicsshape.cc
    physics/csphysicsshape.hh
    physics/iphysicsbasecollider.hh
    physics/iphysicscharactercontroller.hh
    physics/iphysicscapsulecharactercontroller.hh
    physics/iphysicscollider.hh
    physics/iphysicsdynamiccollider.hh
    physics/iphysicsjoint.hh
    physics/iphysicshingejoint.hh
    physics/iphysicsscene.hh
    physics/iphysicsshape.hh
    physics/iphysicsstaticcollider.hh
    physics/iphysicssystem.hh
    physics/iphysicsstaticcollider.hh
    physics/iphysicstriggercollider.hh
    )

source_group(
    "cobalt\\physics"
    FILES
    ${SOURCE_PHYSICS}
    )

set(SOURCE_WINDOW
    window/ikeyboard.hh
    window/imouse.hh
    window/iwindow.hh
    )

source_group(
    "cobalt\\window"
    FILES
    ${SOURCE_WINDOW}
    )


set(ALL_SOURCES
    ${SOURCE}
    ${SOURCE_ANIMATION}
    ${SOURCE_CORE}
    ${SOURCE_CORE_PROPERTY}
    ${SOURCE_CORE_RESOURCE}
    ${SOURCE_ENTITY}
    ${SOURCE_ENTITY_BLUEPRINT}
    ${SOURCE_GRAPHICS}
    ${SOURCE_GRAPHICS_DEFERRED}
    ${SOURCE_GRAPHICS_SCENE}
    ${SOURCE_GRAPHICS_SHADERGRAPH}
    ${SOURCE_LOADERS}
    ${SOURCE_MATH}
    ${SOURCE_PHYSICS}
    ${SOURCE_WINDOW}
    )


add_library(CobaltSKY SHARED
    ${ALL_SOURCES}
    )

add_dependencies(CobaltSKY ClassGenerator)

install(TARGETS CobaltSKY 
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		)

install(DIRECTORY . DESTINATION include/cobaltsky/cobalt FILES_MATCHING PATTERN "*.hh")

target_link_libraries(CobaltSKY
    TiXml
	CobaltSKY-File
    ${PNG_LIBRARIES}
    )

CS_INSTALL_MOCCER(CobaltSKY cobalt CSE_API ALL_SOURCES)


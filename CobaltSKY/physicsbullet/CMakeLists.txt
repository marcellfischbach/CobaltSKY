

add_definitions(-DCSBULLETPHYSICS_EXPORT)

set(SOURCE
  bulletexport.hh
  bulletcapsulecharactercontroller.cc
  bulletcapsulecharactercontroller.hh
  bulletdynamiccollider.cc
  bulletdynamiccollider.hh
  bullethingejoint.cc
  bullethingejoint.hh
  bulletjoint.cc
  bulletjoint.hh
  bulletscene.cc
  bulletscene.hh
  bulletshape.cc
  bulletshape.hh
  bulletstaticcollider.cc
  bulletstaticcollider.hh
  bulletsystem.cc
  bulletsystem.hh
  bulletmodule.cc
  bulletmodule.hh
  )

source_group(
	"physicsbullet"
	FILES
	${SOURCE}
	)



set(ALL_SOURCES
	${SOURCE}
	)



source_group(
	"physicsbullet\\MOC_sources"
	FILES
	${MOC_SOURCES}
	)

add_library(CobaltSKY-PhysicsBullet SHARED
	${ALL_SOURCES}
	)

target_link_libraries(CobaltSKY-PhysicsBullet
	CobaltSKY
	${BULLET_LIBRARIES}
  )

CS_INSTALL_MOCCER(CobaltSKY-PhysicsBullet physicsbullet CSBULLETPHYSICS_API ALL_SOURCES)


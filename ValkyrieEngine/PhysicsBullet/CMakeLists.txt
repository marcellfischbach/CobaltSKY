

add_definitions(-DVKBULLETPHYSICS_EXPORT)

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
	"PhysicsBullet"
	FILES
	${SOURCE}
	)



set(ALL_SOURCES
	${SOURCE}
	)



source_group(
	"PhysicsBullet\\MOC_sources"
	FILES
	${MOC_SOURCES}
	)

add_library(ValkyrieEngine-PhysicsBullet SHARED
	${ALL_SOURCES}
	)

target_link_libraries(ValkyrieEngine-PhysicsBullet
	ValkyrieEngine
	${BULLET_LIBRARIES}
  )

VK_INSTALL_MOCCER(ValkyrieEngine-PhysicsBullet physicsbullet VKBULLETPHYSICS_API ALL_SOURCES)


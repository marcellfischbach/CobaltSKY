

add_definitions(-DVKBULLET_EXPORT)

set(SOURCE
  Export.hh
  BulletCharacterController.cc
  BulletCharacterController.hh
  BulletDynamicCollider.cc
  BulletDynamicCollider.hh
  BulletJoints.cc
  BulletJoints.hh
  BulletScene.cc
  BulletScene.hh
  BulletShape.cc
  BulletShape.hh
  BulletStaticCollider.cc
  BulletStaticCollider.hh
  BulletSystem.cc
  BulletSystem.hh
  Module.cc
  Module.hh
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

VK_INSTALL_MOCCER(ValkyrieEngine-PhysicsBullet PhysicsBullet VKBULLET_API ALL_SOURCES)


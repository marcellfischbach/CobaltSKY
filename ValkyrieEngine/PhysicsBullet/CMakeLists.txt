

add_definitions(-DVKBULLET_EXPORT)

set(SOURCE
  Export.hh
  BulletCharacterController.cc
  BulletCharacterController.hh
  BulletDynamicCollider.cc
  BulletDynamicCollider.hh
  BulletScene.cc
  BulletScene.hh
  BulletShape.cc
  BulletShape.hh
  BulletStaticCollider.cc
  BulletStaticCollider.hh
  BulletSystem.cc
  BulletSystem.hh
  )


source_group(
	"PhysicsBullet"
	FILES
	${SOURCE}
	)



set(ALL_SOURCES
	${SOURCE}
	)

VK_MOC(PhysicsBullet VKBULLET_API ALL_SOURCES MOC_SOURCES)


source_group(
	"PhysicsBullet\\MOC_sources"
	FILES
	${MOC_SOURCES}
	)

add_library(ValkyrieEngine-PhysicsBullet SHARED
	${ALL_SOURCES}
	${MOC_SOURCES}
	)

target_link_libraries(ValkyrieEngine-PhysicsBullet
	ValkyrieEngine
	${BULLET_LIBRARIES}
  )

VK_INSTALL_MOCCER(ValkyrieEngine-PhysicsBullet PhysicsBullet VKBULLET_API ALL_SOURCES)




set(SOURCE
  main.cc
  )

source_group(
	"ValkyrieRunner"
	FILES
	${SOURCE}
	)

set(ALL_SOURCES
	${SOURCE}
	)

add_executable(ValkyrieRunner
	${ALL_SOURCES}
	)

target_link_libraries(ValkyrieRunner
  ValkyrieEngine
  ValkyrieEngine-SDLWindow
  ValkyrieEngine-GraphicsGL4
  ValkyrieEngine-PhysicsBullet
  )


VK_INSTALL_MOCCER(ValkyrieRunner Runner " " ALL_SOURCES)


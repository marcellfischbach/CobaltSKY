

set(SOURCE
  main.cc
  )

source_group(
	"runner"
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
  ValkyrieEngine-WindowSDL
  ValkyrieEngine-GraphicsGL4
  ValkyrieEngine-PhysicsBullet
  )


CS_INSTALL_MOCCER(ValkyrieRunner runner " " ALL_SOURCES)


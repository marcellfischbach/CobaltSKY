

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

add_executable(CobaltSKY-Runner
	${ALL_SOURCES}
	)

target_link_libraries(CobaltSKY-Runner
  CobaltSKY
  CobaltSKY-WindowSDL
  CobaltSKY-GraphicsGL4
  CobaltSKY-PhysicsBullet
  )


CS_INSTALL_MOCCER(CobaltSKY-Runner runner " " ALL_SOURCES)


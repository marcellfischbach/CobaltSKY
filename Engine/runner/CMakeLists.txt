

set(SOURCE
  event.cc
  event.hh
  main.cc
  runnermodule.cc
  runnermodule.hh
  resource.rc
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


install(TARGETS CobaltSKY-Runner
		RUNTIME DESTINATION bin
		)

target_link_libraries(CobaltSKY-Runner
  CobaltSKY
  CobaltSKY-WindowSDL
  CobaltSKY-GraphicsGL4
  CobaltSKY-PhysicsBullet
  )




CS_MOC(CobaltSKY-Runner runner)

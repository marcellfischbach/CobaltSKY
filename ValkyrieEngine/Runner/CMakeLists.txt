

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

VK_MOC(Runner " " ALL_SOURCES MOC_SOURCES)


source_group(
	"MOC_sources"
	FILES
	${MOC_SOURCES}
	)

add_executable(ValkyrieRunner
	${ALL_SOURCES}
	${MOC_SOURCES}
	)

target_link_libraries(ValkyrieRunner
  ValkyrieEngine
  ValkyrieEngine-SDLWindow
  ValkyrieEngine-GraphicsGL4
  )


VK_INSTALL_MOCCER(ValkyrieRunner Runner " " ALL_SOURCES)


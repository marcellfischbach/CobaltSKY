

set(SOURCE
  SDLWindow.hh
  SDLWindow.cc
  )

source_group(
	"ValkyrieEngine-SDLWindow"
	FILES
	${SOURCE}
	)

set(ALL_SOURCES
	${SOURCE}
	)

VK_MOC(SDLWindow ALL_SOURCES MOC_SOURCES)


source_group(
	"ValkyrieEngine-SDLWindow\\MOC_sources"
	FILES
	${MOC_SOURCES}
	)

add_library(ValkyrieEngine-SDLWindow SHARED
	${ALL_SOURCES}
	${MOC_SOURCES}
	)

target_link_libraries(ValkyrieEngine-SDLWindow
  ValkyrieEngine)

VK_INSTALL_MOCCER(ValkyrieEngine-SDLWindow SDLWindow ALL_SOURCES)


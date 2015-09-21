

add_definitions(-DVKSDL_EXPORT)

set(SOURCE
  Export.hh
  SDLKeyboard.cc
  SDLKeyboard.hh
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

VK_MOC(SDLWindow VKSDL_API ALL_SOURCES MOC_SOURCES)


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
  ValkyrieEngine
	${SDL_LIBRARY}
	${OPENGL_LIBRARIES}
  )

VK_INSTALL_MOCCER(ValkyrieEngine-SDLWindow SDLWindow VKSDL_API ALL_SOURCES)




add_definitions(-DVKWINDOWSDL_EXPORT)

set(SOURCE
  sdlexport.hh
  sdlwindowmodule.cc
  sdlwindowmodule.hh
  sdlbuttonMap.hh
  sdlkeymap.hh
  sdlkeyboard.cc
  sdlkeyboard.hh
  sdlmouse.cc
  sdlmouse.hh
  sdlwindow.hh
  sdlwindow.cc
  )

source_group(
	"windowsdl"
	FILES
	${SOURCE}
	)

set(ALL_SOURCES
	${SOURCE}
	)

add_library(ValkyrieEngine-WindowSDL SHARED
	${ALL_SOURCES}
	)

target_link_libraries(ValkyrieEngine-WindowSDL
  ValkyrieEngine
	${SDL_LIBRARY}
	${OPENGL_LIBRARIES}
  )

VK_INSTALL_MOCCER(ValkyrieEngine-WindowSDL SDLWindow VKWINDOWSDL_API ALL_SOURCES)


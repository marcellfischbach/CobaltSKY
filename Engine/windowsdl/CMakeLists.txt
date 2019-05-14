

add_definitions(-DCSWINDOWSDL_EXPORT)

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

add_library(CobaltSKY-WindowSDL SHARED
	${ALL_SOURCES}
	)


install(TARGETS CobaltSKY-WindowSDL
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		)

install(DIRECTORY . DESTINATION include/cobaltsky/windowsdl FILES_MATCHING PATTERN "*.hh")


target_link_libraries(CobaltSKY-WindowSDL
  CobaltSKY
	${SDL_LIBRARY}
	${OPENGL_LIBRARIES}
  )

CS_MOC(CobaltSKY-WindowSDL windowsdl CSWINDOWSDL_API ALL_SOURCES)


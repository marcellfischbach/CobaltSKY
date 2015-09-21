

add_definitions(-DVKE_EXPORT)

set(SOURCE
  Engine.hh
  Engine.cc
  Enums.hh
  Export.hh
  Types.hh
  )

source_group(
	"ValkyrieEngine"
	FILES
	${SOURCE}
	)

set(SOURCE_CORE
  Core/Library.cc
  Core/Library.hh
	Core/Object.cc
	Core/Object.hh
  Core/Thread.cc
  Core/Thread.hh
	)

source_group(
	"ValkyrieEngine\\Core"
	FILES
	${SOURCE_CORE}
	)

set(SOURCE_GRAPHICS
  Graphics/IRenderer.hh
  Graphics/IVertexBuffer.hh
  )

  
source_group(
	"ValkyrieEngine\\Graphics"
	FILES
	${SOURCE_GRAPHICS}
	)

set(SOURCE_WINDOW
  Window/IKeyboard.hh
  Window/IWindow.hh
	)

source_group(
	"ValkyrieEngine\\Window"
	FILES
	${SOURCE_WINDOW}
	)


set(ALL_SOURCES
  ${SOURCE}
	${SOURCE_CORE}
  ${SOURCE_GRAPHICS}
  ${SOURCE_WINDOW}
	)

VK_MOC(Valkyrie VKE_API ALL_SOURCES MOC_SOURCES)


source_group(
	"ValkyrieEngine\\MOC_sources"
	FILES
	${MOC_SOURCES}
	)

add_library(ValkyrieEngine SHARED
	${ALL_SOURCES}
	${MOC_SOURCES}
	)

VK_INSTALL_MOCCER(ValkyrieEngine Valkyrie VKE_API ALL_SOURCES)


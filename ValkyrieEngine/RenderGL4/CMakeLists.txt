

add_definitions(-DVKGL4_EXPORT)

set(SOURCE
  Export.hh
  MappingGL4.hh
  RendererGL4.cc
  RendererGL4.hh
  VertexBufferGL4.cc
  VertexBufferGL4.hh
  )

source_group(
	"ValkyrieEngine-RenderGL4"
	FILES
	${SOURCE}
	)

set(ALL_SOURCES
	${SOURCE}
	)

VK_MOC(RenderGL4 VKGL4_API ALL_SOURCES MOC_SOURCES)


source_group(
	"ValkyrieEngine-RenderGL4\\MOC_sources"
	FILES
	${MOC_SOURCES}
	)

add_library(ValkyrieEngine-RenderGL4 SHARED
	${ALL_SOURCES}
	${MOC_SOURCES}
	)

target_link_libraries(ValkyrieEngine-RenderGL4
  ValkyrieEngine
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
  )

VK_INSTALL_MOCCER(ValkyrieEngine-RenderGL4 RenderGL4 VKGL4_API ALL_SOURCES)


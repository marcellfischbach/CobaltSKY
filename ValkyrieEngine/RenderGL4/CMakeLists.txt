

add_definitions(-DVKGL4_EXPORT)

set(SOURCE
  Enums.hh
  Export.hh
  IndexBufferGL4.cc
  IndexBufferGL4.hh
  MappingGL4.hh
  RendererGL4.cc
  RendererGL4.hh
  RenderTargetGL4.cc
  RenderTargetGL4.hh
  Shader.cc
  Shader.hh
  ShaderLoader.cc
  ShaderLoader.hh
  TextureGL4.cc
  TextureGL4.hh
  VertexBufferGL4.cc
  VertexBufferGL4.hh
  VertexDeclarationGL4.cc
  VertexDeclarationGL4.hh
  )

source_group(
	"ValkyrieEngine-RenderGL4"
	FILES
	${SOURCE}
	)


set(SOURCE_DEFERRED
  Deferred/DeferredFrameProcessor.cc
  Deferred/DeferredFrameProcessor.hh
  Deferred/GBuffer.cc
  Deferred/GBuffer.hh
  Deferred/LightRenderer.cc
  Deferred/LightRenderer.hh
  )
  
source_group(
	"ValkyrieEngine-RenderGL4\\Deferred"
	FILES
	${SOURCE_DEFERRED}
	)


set(ALL_SOURCES
	${SOURCE}
  ${SOURCE_DEFERRED}
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


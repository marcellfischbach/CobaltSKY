

add_definitions(-DVKGL4_EXPORT)

set(SOURCE
  Enums.hh
  Export.hh
  GraphicsGL4.cc
  GraphicsGL4.hh
  IndexBufferGL4.cc
  IndexBufferGL4.hh
  MappingGL4.hh
  Module.cc
  Module.hh
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
	"GraphicsGL4"
	FILES
	${SOURCE}
	)


set(SOURCE_DEFERRED
  Deferred/DefaultCollector.cc
  Deferred/DefaultCollector.hh
  Deferred/DeferredFrameProcessor.cc
  Deferred/DeferredFrameProcessor.hh
  Deferred/GBuffer.cc
  Deferred/GBuffer.hh
  Deferred/LightRenderer.cc
  Deferred/LightRenderer.hh
  )
  
source_group(
	"GraphicsGL4\\Deferred"
	FILES
	${SOURCE_DEFERRED}
	)


set(SOURCE_SHADERGRAPH
  ShaderGraph/GBufferGL4.cc
  ShaderGraph/ShaderGraphGL4.cc
  ShaderGraph/ShaderGraphGL4.hh
  ShaderGraph/ShaderGraphNodeGL4.cc
  ShaderGraph/ShaderGraphNodeGL4.hh
  ShaderGraph/ShadowPSSM.cc
  )
  
source_group(
	"GraphicsGL4\\ShaderGraph"
	FILES
	${SOURCE_SHADERGRAPH}
	)


set(ALL_SOURCES
	${SOURCE}
  ${SOURCE_DEFERRED}
	${SOURCE_SHADERGRAPH}
	)


add_library(ValkyrieEngine-GraphicsGL4 SHARED
	${ALL_SOURCES}
	)

target_link_libraries(ValkyrieEngine-GraphicsGL4
  ValkyrieEngine
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
  )

VK_INSTALL_MOCCER(ValkyrieEngine-GraphicsGL4 GraphicsGL4 VKGL4_API ALL_SOURCES)


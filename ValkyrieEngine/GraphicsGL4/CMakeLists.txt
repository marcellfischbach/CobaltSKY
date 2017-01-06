

add_definitions(-DVKGRAPHICSGL4_EXPORT)


set(SOURCE
  gl4enums.hh
  gl4export.hh
  gl4defines.hh
  gl4graphics.cc
  gl4graphics.hh
  gl4indexbuffer.cc
  gl4indexbuffer.hh
  gl4mapping.hh
  gl4module.cc
  gl4module.hh
  gl4program.cc
  gl4program.hh
  gl4programloader.cc
  gl4programloader.hh
  gl4rendertarget.cc
  gl4rendertarget.hh
  gl4sampler.cc
  gl4sampler.hh
  gl4shader.cc
  gl4shader.hh
  gl4shaderattribute.cc
  gl4shaderattribute.hh
  gl4shaderloader.cc
  gl4shaderLoader.hh
  gl4shaderstream.cc
  gl4shaderstream.hh
  gl4texture.cc
  gl4texture.hh
  gl4texture2d.cc
  gl4texture2d.hh
  gl4texture2darray.cc
  gl4texture2darray.hh
  gl4texturecube.cc
  gl4texturecube.hh
  gl4vertexbuffer.cc
  gl4vertexbuffer.hh
  gl4vertexdeclaration.cc
  gl4vertexdeclaration.hh
  )


source_group(
	"GraphicsGL4"
	FILES
	${SOURCE}
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

VK_INSTALL_MOCCER(ValkyrieEngine-GraphicsGL4 GraphicsGL4 VKGRAPHICSGL4_API ALL_SOURCES)


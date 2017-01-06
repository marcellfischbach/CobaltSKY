

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
  ShaderGraph/gl4sgadd.cc
  ShaderGraph/gl4sgadd.hh
  ShaderGraph/gl4sgconstfloat1.cc
  ShaderGraph/gl4sgconstfloat1.hh
  ShaderGraph/gl4sgconstfloat2.cc
  ShaderGraph/gl4sgconstfloat2.hh
  ShaderGraph/gl4sgconstfloat3.cc
  ShaderGraph/gl4sgconstfloat3.hh
  ShaderGraph/gl4sgconstfloat4.cc
  ShaderGraph/gl4sgconstfloat4.hh
  ShaderGraph/gl4sgcross.cc
  ShaderGraph/gl4sgcross.hh
  ShaderGraph/gl4sgdefaulttexturecoordinate.cc
  ShaderGraph/gl4sgdefaulttexturecoordinate.hh
  ShaderGraph/gl4sgdiv.cc
  ShaderGraph/gl4sgdiv.hh
  ShaderGraph/gl4sgdot.cc
  ShaderGraph/gl4sgdot.hh
  ShaderGraph/gl4sgfloat2.cc
  ShaderGraph/gl4sgfloat2.hh
  ShaderGraph/gl4sgfloat3.cc
  ShaderGraph/gl4sgfloat3.hh
  ShaderGraph/gl4sgfloat4.cc
  ShaderGraph/gl4sgfloat4.hh
  ShaderGraph/gl4sgfloateval.hh
  ShaderGraph/gl4sglerp.cc
  ShaderGraph/gl4sglerp.hh
  ShaderGraph/gl4sgmul.cc
  ShaderGraph/gl4sgmul.hh
  ShaderGraph/gl4sgnode.cc
  ShaderGraph/gl4sgnode.hh
  ShaderGraph/gl4sgshadergraph.cc
  ShaderGraph/gl4sgshadergraph.hh
  ShaderGraph/gl4sgshadergraph_gbuffer.cc
  ShaderGraph/gl4sgshadergraph_shadowpssm.cc
  ShaderGraph/gl4sgshadergraphctx.hh
  ShaderGraph/gl4sgshadergraphctx.cc
  ShaderGraph/gl4sgsplitfloat2.cc
  ShaderGraph/gl4sgsplitfloat2.hh
  ShaderGraph/gl4sgsplitfloat3.cc
  ShaderGraph/gl4sgsplitfloat3.hh
  ShaderGraph/gl4sgsplitfloat4.cc
  ShaderGraph/gl4sgsplitfloat4.hh
  ShaderGraph/gl4sgsub.cc
  ShaderGraph/gl4sgsub.hh
  ShaderGraph/gl4sgtexture2d.cc
  ShaderGraph/gl4sgtexture2d.hh
  ShaderGraph/gl4sgvar.cc
  ShaderGraph/gl4sgvar.hh
  ShaderGraph/gl4sgvarfloat1.hh
  ShaderGraph/gl4sgvarfloat2.hh
  ShaderGraph/gl4sgvarfloat3.hh
  ShaderGraph/gl4sgvarfloat4.hh
  )
  
source_group(
	"GraphicsGL4\\shadergraph"
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


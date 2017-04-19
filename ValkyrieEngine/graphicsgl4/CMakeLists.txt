

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
	"graphicsgl4"
	FILES
	${SOURCE}
	)




set(SOURCE_SHADERGRAPH
  shadergraph/gl4sgadd.cc
  shadergraph/gl4sgadd.hh
  shadergraph/gl4sgconstfloat1.cc
  shadergraph/gl4sgconstfloat1.hh
  shadergraph/gl4sgconstfloat2.cc
  shadergraph/gl4sgconstfloat2.hh
  shadergraph/gl4sgconstfloat3.cc
  shadergraph/gl4sgconstfloat3.hh
  shadergraph/gl4sgconstfloat4.cc
  shadergraph/gl4sgconstfloat4.hh
  shadergraph/gl4sgcross.cc
  shadergraph/gl4sgcross.hh
  shadergraph/gl4sgdefaulttexturecoordinate.cc
  shadergraph/gl4sgdefaulttexturecoordinate.hh
  shadergraph/gl4sgdiv.cc
  shadergraph/gl4sgdiv.hh
  shadergraph/gl4sgdot.cc
  shadergraph/gl4sgdot.hh
  shadergraph/gl4sgfloat2.cc
  shadergraph/gl4sgfloat2.hh
  shadergraph/gl4sgfloat3.cc
  shadergraph/gl4sgfloat3.hh
  shadergraph/gl4sgfloat4.cc
  shadergraph/gl4sgfloat4.hh
  shadergraph/gl4sgfloateval.hh
  shadergraph/gl4sglerp.cc
  shadergraph/gl4sglerp.hh
  shadergraph/gl4sgnormalize.cc
  shadergraph/gl4sgnormalize.hh
  shadergraph/gl4sgmul.cc
  shadergraph/gl4sgmul.hh
  shadergraph/gl4sgnode.cc
  shadergraph/gl4sgnode.hh
  shadergraph/gl4sgshadergraph.cc
  shadergraph/gl4sgshadergraph.hh
  shadergraph/gl4sgshadergraph_gbuffer.cc
  shadergraph/gl4sgshadergraph_shadowpssm.cc
  shadergraph/gl4sgshadergraphctx.hh
  shadergraph/gl4sgshadergraphctx.cc
  shadergraph/gl4sgsplitfloat2.cc
  shadergraph/gl4sgsplitfloat2.hh
  shadergraph/gl4sgsplitfloat3.cc
  shadergraph/gl4sgsplitfloat3.hh
  shadergraph/gl4sgsplitfloat4.cc
  shadergraph/gl4sgsplitfloat4.hh
  shadergraph/gl4sgsub.cc
  shadergraph/gl4sgsub.hh
  shadergraph/gl4sgtexture2d.cc
  shadergraph/gl4sgtexture2d.hh
  shadergraph/gl4sgvar.cc
  shadergraph/gl4sgvar.hh
  shadergraph/gl4sgvarfloat1.hh
  shadergraph/gl4sgvarfloat2.hh
  shadergraph/gl4sgvarfloat3.hh
  shadergraph/gl4sgvarfloat4.hh
  )
  
source_group(
	"graphicsgl4\\shadergraph"
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

VK_INSTALL_MOCCER(ValkyrieEngine-GraphicsGL4 graphicsgl4 VKGRAPHICSGL4_API ALL_SOURCES)


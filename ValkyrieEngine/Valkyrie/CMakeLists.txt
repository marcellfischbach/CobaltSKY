

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
  Core/FileInfo.cc
  Core/FileInfo.hh
  Core/FileStd.cc
  Core/FileStd.hh
  Core/IFile.hh
  Core/Library.cc
  Core/Library.hh
	Core/Object.cc
	Core/Object.hh
  Core/ResourceManager.cc
  Core/ResourceManager.hh
  Core/Settings.cc
  Core/Settings.hh
  Core/String.hh
  Core/Thread.cc
  Core/Thread.hh
  Core/VFS.cc
  Core/VFS.hh
  Core/Xml.cc
  Core/Xml.hh
	)


source_group(
	"ValkyrieEngine\\Core"
	FILES
	${SOURCE_CORE}
	)

set(SOURCE_GRAPHICS
  Graphics/IIndexBuffer.hh
  Graphics/Image.cc
  Graphics/Image.hh
  Graphics/ImageLoader.cc
  Graphics/ImageLoader.hh
  Graphics/IRenderer.hh
  Graphics/IShader.hh
  Graphics/IVertexBuffer.hh
  Graphics/IVertexDeclaration.hh
  Graphics/Material.cc
  Graphics/Material.hh
  Graphics/MaterialLoader.cc
  Graphics/MaterialLoader.hh
  Graphics/Mesh.cc
  Graphics/Mesh.hh
  Graphics/ShaderID.cc
  Graphics/ShaderID.hh
  )

  
source_group(
	"ValkyrieEngine\\Graphics"
	FILES
	${SOURCE_GRAPHICS}
	)
	
set(SOURCE_GRAPHICS_SCENE
  Graphics/Scene/GeometryNode.cc
  Graphics/Scene/GeometryNode.hh
  Graphics/Scene/GroupNode.cc
  Graphics/Scene/GroupNode.hh
	Graphics/Scene/Node.cc
	Graphics/Scene/Node.hh
  Graphics/Scene/Scan.hh
  Graphics/Scene/SpatialNode.cc
  Graphics/Scene/SpatialNode.hh
	)
	
source_group(
	"ValkyrieEngine\\Graphics\\Scene"
	FILES
	${SOURCE_GRAPHICS_SCENE}
	)
	
set(SOURCE_MATH
  Math/Clipper.cc
  Math/Clipper.hh
	Math/BoundingVolume.cc
	Math/BoundingVolume.hh
	Math/Matrix.hh
  Math/Plane.hh
	Math/Vector.hh
	)
	
source_group(
	"ValkyrieEngine\\Math"
	FILES
	${SOURCE_MATH}
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
	${SOURCE_GRAPHICS_SCENE}
	${SOURCE_MATH}
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

target_link_libraries(ValkyrieEngine
  TiXml
  ${PNG_LIBRARIES}
  )

VK_INSTALL_MOCCER(ValkyrieEngine Valkyrie VKE_API ALL_SOURCES)


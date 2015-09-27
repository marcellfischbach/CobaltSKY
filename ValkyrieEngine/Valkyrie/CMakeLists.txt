

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


set(SOURCE_ASSETS
  Assets/AssetManager.cc
  Assets/AssetManager.hh
	)

source_group(
	"ValkyrieEngine\\Assets"
	FILES
	${SOURCE_ASSETS}
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
  Graphics/IRenderer.hh
  Graphics/IShader.hh
  Graphics/IVertexBuffer.hh
  Graphics/IVertexDeclaration.hh
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
	${SOURCE_ASSETS}
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

target_link_libraries(ValkyrieEngine
  TiXml
  )

VK_INSTALL_MOCCER(ValkyrieEngine Valkyrie VKE_API ALL_SOURCES)


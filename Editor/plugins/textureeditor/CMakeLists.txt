
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)



set(ALL_SOURCES
	textureeditor.cc
	textureeditor.hh
	textureeditorexport.hh
	textureeditorfactory.cc
	textureeditorfactory.hh
	textureeditormodule.cc
	textureeditormodule.hh
	)



add_definitions(-DTEXTURE_EDITOR_EXPORT)


add_library(
	PluginTextureEditor
	SHARED
	${ALL_SOURCES}
	)


add_dependencies(PluginTextureEditor csmoc)

target_link_libraries(
	PluginTextureEditor
    Qt5::Core
    Qt5::Widgets
	EditorCore
	EditorModel

	)

CS_MOC(PluginTextureEditor plugins/textureeditor)



find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


set(ALL_SOURCES
	samplereditor.cc
	samplereditor.hh
	samplereditorexport.hh
	samplereditorfactory.cc
	samplereditorfactory.hh
	samplereditorfile.cc
	samplereditorfile.hh
	samplereditormodule.cc
	samplereditormodule.hh
	samplereditorproperties.cc
	samplereditorproperties.hh
	samplereditorproperties.ui
	samplereditorwidget.cc
	samplereditorwidget.hh
	)



add_definitions(-DSAMPLER_EDITOR_EXPORT)


add_library(
	PluginSamplerEditor
	SHARED
	${ALL_SOURCES}
	)


add_dependencies(PluginSamplerEditor csmoc)

target_link_libraries(
	PluginSamplerEditor
    Qt5::Core
    Qt5::Widgets
	EditorCore
	EditorModel

	)

CS_MOC(PluginSamplerEditor plugins/samplereditor)



find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)



set(ALL_SOURCES
	abstracteditor.cc
	abstracteditor.hh
	editor.cc
	editor.hh
	editorwindow.cc
	editorwindow.hh
	export.hh
	)


add_definitions(-DEDITOR_CORE_EXPORT)


add_library(
	EditorCore
	SHARED
	${ALL_SOURCES}
	)


add_dependencies(EditorCore csmoc)

target_link_libraries(
	EditorCore
    Qt5::Core
    Qt5::Widgets
	EditorModel
	)

CS_MOC(EditorCore editorcore)



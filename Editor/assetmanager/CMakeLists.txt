
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)


set(ALL_SOURCES
	assetmanager.cc
	assetmanager.hh
	assetmanagerexport.hh
	tree/assetmanagertreemodel.cc
	tree/assetmanagertreemodel.hh
	tree/assetmanagertreenode.cc
	tree/assetmanagertreenode.hh
	tree/assetmanagertreeview.cc
	tree/assetmanagertreeview.hh
	)


add_definitions(-DASSET_MANAGER_EXPORT)


add_library(
	EditorAssetManager
	SHARED
	${ALL_SOURCES}
	)


add_dependencies(EditorAssetManager ClassGenerator)

target_link_libraries(
	EditorAssetManager
    Qt5::Core
    Qt5::Widgets
	EditorModel
	)

CS_INSTALL_MOCCER(EditorAssetManager assetmanager ASSET_MANAGER_API ALL_SOURCES)


add_executable(
	EditorAssetManagerTest
	main.cc
	)

	
add_dependencies(EditorAssetManagerTest ClassGenerator)

target_link_libraries(
	EditorAssetManagerTest
    Qt5::Core
    Qt5::Widgets
	EditorAssetManager
	)

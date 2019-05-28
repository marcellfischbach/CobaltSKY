
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


set(ALL_SOURCES
	assetmanager.cc
	assetmanager.hh
	assetmanagerexport.hh
	assetmanagermodule.cc
	assetmanagermodule.hh
	assetmanagertoolview.cc
	assetmanagertoolview.hh
	resources.qrc

	list/assetmanagerlistlistmodel.cc
	list/assetmanagerlistlistmodel.hh
	list/assetmanagerlistlistnode.cc
	list/assetmanagerlistlistnode.hh
	list/assetmanagerlistview.cc
	list/assetmanagerlistview.hh
	list/assetmanagerlisttreemodel.cc
	list/assetmanagerlisttreemodel.hh
	list/assetmanagerlisttreenode.cc
	list/assetmanagerlisttreenode.hh

	menu/imenuitemfactory.hh
	menu/menubuilder.cc
	menu/menubuilder.hh
	menu/menuitemdeclaration.cc
	menu/menuitemdeclaration.hh
	menu/menuitemfactories.cc
	menu/menuitemfactories.hh
	menu/menuitemhandler.cc
	menu/menuitemhandler.hh

	menu/items/delete/deletedialog.cc
	menu/items/delete/deletedialog.hh
	menu/items/delete/deletedialog.ui
	menu/items/delete/deletemenuitem.cc
	menu/items/delete/deletemenuitem.hh

	menu/items/newfolder/newfolderdialog.cc
	menu/items/newfolder/newfolderdialog.hh
	menu/items/newfolder/newfolderdialog.ui
	menu/items/newfolder/newfoldermenuitem.cc
	menu/items/newfolder/newfoldermenuitem.hh


	menu/items/rename/renamedialog.cc
	menu/items/rename/renamedialog.hh
	menu/items/rename/renamedialog.ui
	menu/items/rename/renamemenuitem.cc
	menu/items/rename/renamemenuitem.hh



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


add_dependencies(EditorAssetManager csmoc)

target_link_libraries(
	EditorAssetManager
    Qt5::Core
    Qt5::Widgets
	EditorCore
	EditorModel

	)

CS_MOC(EditorAssetManager assetmanager)


add_executable(
	EditorAssetManagerTest
	main.cc
	)

	
add_dependencies(EditorAssetManagerTest csmoc)

target_link_libraries(
	EditorAssetManagerTest
    Qt5::Core
    Qt5::Widgets
	EditorAssetManager
	)

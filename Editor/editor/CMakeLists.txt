
find_package(Qt5 COMPONENTS Core Widgets OpenGL Xml REQUIRED)
#find_package(Qt5Core)
#find_package(Qt5Widgets)
#find_package(Qt5OpenGL)
#find_package(Qt5Xml)
#find_package(assimp)

include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})

message("QtCore   : ${Qt5Core_INCLUDE_DIRS}")
message("QtWidgets: ${Qt5Widgets_INCLUDE_DIRS}")
message("QtOpenGL : ${Qt5OpenGL_INCLUDE_DIRS}")
message("QtXml    : ${Qt5Xml_INCLUDE_DIRS}")

set(CMAKE_AUTOMOC ON)

add_definitions(-DEDITOR_EXPORT)



set(EDITOR_SOURCES
    abstractasseteditor.cc
    abstractasseteditor.hh
    abstractdockitem.cc
    abstractdockitem.hh
    assetdescriptor.cc
    assetdescriptor.hh
    basicdockitem.cc
    basicdockitem.hh
    cobalt.css
    dockitems.hh
	flat.css
	flat_menu.css
    editor.cc
    editor.hh
	editorfactorymanager.cc
	editorfactorymanager.hh
    editorimage.cc
    editorimage.hh
    editormodule.cc
    editormodule.hh
    editorresourcemanager.cc
    editorresourcemanager.hh
    eventbus.cc
    eventbus.hh
    glcontext.cc
    glcontext.hh
    guitest.cc
    guitest.hh
    iasseteditor.hh
    iasseteditorfactory.hh
	iassetimporter.hh
	iassetimporterfactory.hh
    idockitem.hh
	importerfactorymanager.cc
	importerfactorymanager.hh
    mainwindow.cc
    mainwindow.hh
	mainwindowmenubutton.cc
	mainwindowmenubutton.hh
	mainwindowmenupanel.cc
	mainwindowmenupanel.hh
	mainwindowsidepanel.cc
	mainwindowsidepanel.hh
    mimehelper.cc
    mimehelper.hh
    renderwidget.cc
    renderwidget.hh
    )

set(EDITOR_UI
    guitest.ui
    mainwindow.ui
    )

source_group(
    "editor"
    FILES
    ${EDITOR_SOURCES}
    ${EDITOR_UI}
    )


set(ASSETMANAGER_SOURCES
    assetmanager/assetmanagerassetwriter.cc
    assetmanager/assetmanagerassetwriter.hh
    assetmanager/assetmanagercontentitemdelegate.cc
    assetmanager/assetmanagercontentitemdelegate.hh
    assetmanager/assetmanagercontentmodel.cc
    assetmanager/assetmanagercontentmodel.hh
    assetmanager/assetmanagercontentmodelentry.cc
    assetmanager/assetmanagercontentmodelentry.hh
    assetmanager/assetmanagerdock.cc
    assetmanager/assetmanagerdock.hh
    assetmanager/assetmanagerfoldermodel.cc
    assetmanager/assetmanagerfoldermodel.hh
	assetmanager/assetmanagermenubutton.cc
	assetmanager/assetmanagermenubutton.hh
    assetmanager/assetmanagerresourcescanner.cc
    assetmanager/assetmanagerresourcescanner.hh
    assetmanager/assetmanagerwidget.cc
    assetmanager/assetmanagerwidget.hh
    )

set(ASSETMANAGER_UI
    assetmanager/assetmanagercontentitemdelegatewidget.ui
    assetmanager/assetmanagerwidget.ui
    )

source_group(
    "editor\\assetmanager"
    FILES
    ${ASSETMANAGER_SOURCES}
    ${ASSETMANAGER_UI}
    )



set(ASSETMANAGER_ACTIONS_SOURCES
    assetmanager/actions/assetmanageractioncallback.hh
    )

    
source_group(
    "editor\\assetmanager\\actions"
    FILES
    ${ASSETMANAGER_ACTIONS_SOURCES}
    )


set(ASSETMANAGER_ACTIONS_DELETE_SOURCES
    assetmanager/actions/delete/assetmanagerdeleteaction.cc
    assetmanager/actions/delete/assetmanagerdeleteaction.hh
    assetmanager/actions/delete/assetmanagerdeletedialog.cc
    assetmanager/actions/delete/assetmanagerdeletedialog.hh
    )

set(ASSETMANAGER_ACTIONS_DELETE_UI
	assetmanager/actions/delete/assetmanagerdeletedialog.ui
    )
    

source_group(
    "editor\\assetmanager\\actions\\delete"
    FILES
    ${ASSETMANAGER_ACTIONS_DELETE_SOURCES}
    ${ASSETMANAGER_ACTIONS_DELETE_UI}
    )


set(ASSETMANAGER_ACTIONS_IMPORT_SOURCES
	assetmanager/actions/import/assetmanagerimportaction.cc
	assetmanager/actions/import/assetmanagerimportaction.hh
    assetmanager/actions/import/assetmanagerimporterdialog.cc
    assetmanager/actions/import/assetmanagerimporterdialog.hh
    assetmanager/actions/import/assetmanagerimporterpage.cc
    assetmanager/actions/import/assetmanagerimporterpage.hh
    assetmanager/actions/import/assetmanagerimporterlistmodel.cc
    assetmanager/actions/import/assetmanagerimporterlistmodel.hh
    )

set(ASSETMANAGER_ACTIONS_IMPORT_UI
    assetmanager/actions/import/assetmanagerimporterdialog.ui
    assetmanager/actions/import/assetmanagerimporterpage.ui
    )


source_group(
    "editor\\assetmanager\\actions\\import"
    FILES
    ${ASSETMANAGER_ACTIONS_IMPORT_SOURCES}
    ${ASSETMANAGER_ACTIONS_IMPORT_UI}
    )

set(ASSETMANAGER_ACTIONS_NEW_SOURCES
    assetmanager/actions/new/assetmanagernewassetaction.cc
    assetmanager/actions/new/assetmanagernewassetaction.hh
    assetmanager/actions/new/assetmanagernewassetdialog.cc
    assetmanager/actions/new/assetmanagernewassetdialog.hh
    )

set(ASSETMANAGER_ACTIONS_NEW_UI
	assetmanager/actions/new/assetmanagernewassetdialog.ui
	)
    
source_group(
    "editor\\assetmanager\\actions\\new"
    FILES
    ${ASSETMANAGER_ACTIONS_NEW_SOURCES}
    ${ASSETMANAGER_ACTIONS_NEW_UI}
    )

set(ASSETMANAGER_ACTIONS_NEWFOLDER_SOURCES
    assetmanager/actions/newfolder/assetmanagernewfolderaction.cc
    assetmanager/actions/newfolder/assetmanagernewfolderaction.hh
    assetmanager/actions/newfolder/assetmanagernewfolderdialog.cc
    assetmanager/actions/newfolder/assetmanagernewfolderdialog.hh
    )

set(ASSETMANAGER_ACTIONS_NEWFOLDER_UI
	assetmanager/actions/newfolder/assetmanagernewfolderdialog.ui
	)
    
source_group(
    "editor\\assetmanager\\actions\\newfolder"
    FILES
    ${ASSETMANAGER_ACTIONS_NEWFOLDER_SOURCES}
    ${ASSETMANAGER_ACTIONS_NEWFOLDER_UI}
    )


set(ASSETMANAGER_ACTIONS_RENAME_SOURCES
    assetmanager/actions/rename/assetmanagerrenameaction.cc
    assetmanager/actions/rename/assetmanagerrenameaction.hh
    assetmanager/actions/rename/assetmanagerrenamedialog.cc
    assetmanager/actions/rename/assetmanagerrenamedialog.hh
    )

set(ASSETMANAGER_ACTIONS_RENAME_UI
    assetmanager/actions/rename/assetmanagerrenamedialog.ui
    )
    
source_group(
    "editor\\assetmanager\\actions\\rename"
    FILES
    ${ASSETMANAGER_ACTIONS_RENAME_SOURCES}
    ${ASSETMANAGER_ACTIONS_RENAME_UI}
    )



set(ASSETMANAGER_CONTEXTMENU_SOURCES
    assetmanager/contextmenu/contextmenubuilder.cc
    assetmanager/contextmenu/contextmenubuilder.hh
    assetmanager/contextmenu/contextmenucategory.cc
    assetmanager/contextmenu/contextmenucategory.hh
    assetmanager/contextmenu/contextmenuentry.cc
    assetmanager/contextmenu/contextmenuentry.hh
    assetmanager/contextmenu/contextmenuhandler.cc
    assetmanager/contextmenu/contextmenuhandler.hh
    )

set(ASSETMANAGER_CONTEXTMENU_UI
    )
    
source_group(
    "editor\\assetmanager\\contextmenu"
    FILES
    ${ASSETMANAGER_CONTEXTMENU_SOURCES}
    ${ASSETMANAGER_CONTEXTMENU_UI}
    )





set(ASSETMANAGER_MODEL_SOURCES
	assetmanager/model/assetlistmodel.cc
	assetmanager/model/assetlistmodel.hh
	assetmanager/model/foldertreemodel.cc
	assetmanager/model/foldertreemodel.hh
	assetmanager/model/treemodel.cc
	assetmanager/model/treemodel.hh
	assetmanager/model/viewasset.cc
	assetmanager/model/viewasset.hh
	assetmanager/model/viewdatamodel.cc
	assetmanager/model/viewdatamodel.hh
	assetmanager/model/viewentry.cc
	assetmanager/model/viewentry.hh
	assetmanager/model/viewfolder.cc
	assetmanager/model/viewfolder.hh
	assetmanager/model/viewroot.cc
	assetmanager/model/viewroot.hh
	assetmanager/model/viewvfsentry.cc
	assetmanager/model/viewvfsentry.hh
	)


source_group(
    "editor\\assetmanager\\model"
    FILES
    ${ASSETMANAGER_MODEL_SOURCES}
    )


set(ASSETMODEL_SOURCES
	assetmodel/asset.cc
	assetmodel/asset.hh
	assetmodel/entry.cc
	assetmodel/entry.hh
	assetmodel/exceptions.cc
	assetmodel/exceptions.hh
	assetmodel/folder.cc
	assetmodel/folder.hh
	assetmodel/model.cc
	assetmodel/model.hh
	assetmodel/modelsync.cc
	assetmodel/modelsync.hh
	assetmodel/modeltransaction.cc
	assetmodel/modeltransaction.hh
	assetmodel/root.cc
	assetmodel/root.hh
	assetmodel/vfsentry.cc
	assetmodel/vfsentry.hh
	)


source_group(
    "editor\\assetmodel"
    FILES
    ${ASSETMODEL_SOURCES}
    )

	
set(ASSETMODEL_SYNC_SOURCES
	assetmodel/sync/assetmodifier.cc
	assetmodel/sync/assetmodifier.hh
	assetmodel/sync/assetscanner.cc
	assetmodel/sync/assetscanner.hh
	assetmodel/sync/modelscanner.cc
	assetmodel/sync/modelscanner.hh
	assetmodel/sync/securefs.cc
	assetmodel/sync/securefs.hh
	)


source_group(
    "editor\\assetmodel\\sync"
    FILES
    ${ASSETMODEL_SYNC_SOURCES}
    )

set(COMPONENTS_SOURCES
    components/assetresourcelineedit.cc
    components/assetresourcelineedit.hh
    components/assetresourcewidget.cc
    components/assetresourcewidget.hh
	components/baseeditorwidget.cc
	components/baseeditorwidget.hh
    components/qtcsonscreenrendertarget.cc
    components/qtcsonscreenrendertarget.hh
    components/sceneview.cc
    components/sceneview.hh
    components/sceneviewbaseinputhandler.cc
    components/sceneviewbaseinputhandler.hh
    components/sceneviewinputhandler.hh
    )

set(COMPONENTS_UI
    components/assetresourcewidget.ui
    )


source_group(
    "editor\\components"
    FILES
    ${COMPONENTS_SOURCES}
    ${COMPONENTS_UI}
    )

set(COMPONENTS_NODEGRAPH_SOURCES
    components/nodegraph/nodegraphacceptevent.cc
    components/nodegraph/nodegraphacceptevent.hh
    components/nodegraph/nodegraphnode.cc
    components/nodegraph/nodegraphnode.hh
    components/nodegraph/nodegraphnodeanchor.cc
    components/nodegraph/nodegraphnodeanchor.hh
    components/nodegraph/nodegraphnodeheader.cc
    components/nodegraph/nodegraphnodeheader.hh
    components/nodegraph/nodegraphnodeimageproperty.cc
    components/nodegraph/nodegraphnodeimageproperty.hh
    components/nodegraph/nodegraphnodeproperty.cc
    components/nodegraph/nodegraphnodeproperty.hh
    components/nodegraph/nodegraphnodepropertytype.hh
    components/nodegraph/nodegraphnodevalueproperty.cc
    components/nodegraph/nodegraphnodevalueproperty.hh
    components/nodegraph/nodegraphvetoevent.cc
    components/nodegraph/nodegraphvetoevent.hh
    components/nodegraph/nodegraphwidget.cc
    components/nodegraph/nodegraphwidget.hh
    )

    

source_group(
    "editor\\components\\nodegraph"
    FILES
    ${COMPONENTS_NODEGRAPH_SOURCES}
    )

	                   
set(COMPONENTS_PREVIEW_SOURCES
    components/preview/previewcameraflowhandler.cc
    components/preview/previewcameraflowhandler.hh
    components/preview/previewcameraorbithandler.cc
    components/preview/previewcameraorbithandler.hh
    components/preview/previewlightorbithandler.cc
    components/preview/previewlightorbithandler.hh
    components/preview/previewsceneview.cc
    components/preview/previewsceneview.hh
    )


source_group(
    "editor\\components\\preview"
    FILES
    ${COMPONENTS_PREVIEW_SOURCES}
    )


set(EDITORS_STATICMESHEDITOR_SOURCES
    editors/staticmesheditor/staticmesheditor.cc
    editors/staticmesheditor/staticmesheditor.hh
    editors/staticmesheditor/staticmesheditorfactory.cc
    editors/staticmesheditor/staticmesheditorfactory.hh
    editors/staticmesheditor/staticmesheditorpreviewsceneview.cc
    editors/staticmesheditor/staticmesheditorpreviewsceneview.hh
    editors/staticmesheditor/staticmesheditorwidget.cc
    editors/staticmesheditor/staticmesheditorwidget.hh
    )

set(EDITORS_STATICMESHEDITOR_UI
    editors/staticmesheditor/staticmesheditorwidget.ui
    )


source_group(
    "editor\\editors\\staticmesheditor"
    FILES
    ${EDITORS_STATICMESHEDITOR_SOURCES}
    ${EDITORS_STATICMESHEDITOR_UI}
    )


 set(EVENTS_SOURCES
    events/assetaddedevent.cc
    events/assetaddedevent.hh
    events/assetpreviewiconchangedevent.cc
    events/assetpreviewiconchangedevent.hh
    events/assetrenamedevent.cc
    events/assetrenamedevent.hh
    )


source_group(
    "editor\\events"
    FILES
    ${EVENTS_SOURCES}
    )

       


set(LOADERS_SOURCES
    loaders/loaderseditoriconassetcsfloader.cc
    loaders/loaderseditoriconassetcsfloader.hh
    )


source_group(
    "editor\\loaders"
    FILES
    ${LOADERS_SOURCES}
    )





 

set(PROJECT_SOURCES
    project/project.cc
    project/project.hh
	project/projectmenubutton.cc
	project/projectmenubutton.hh
    )

source_group(
    "editor\\project"
    FILES
    ${PROJECT_SOURCES}
    )


set(ALL_UI
    ${EDITOR_UI}
    ${ASSETMANAGER_UI}
	${ASSETMANAGER_ACTIONS_DELETE_UI}
    ${ASSETMANAGER_ACTIONS_IMPORT_UI}
	${ASSETMANAGER_ACTIONS_NEW_UI}
	${ASSETMANAGER_ACTIONS_NEWFOLDER_UI}
    ${ASSETMANAGER_ACTIONS_RENAME_UI}
    ${ASSETMANAGER_CONTEXTMENU_UI}
    ${COMPONENTS_UI}
    ${EDITORS_STATICMESHEDITOR_UI}
    )


qt5_wrap_ui(ALL_UI_SOURCES ${ALL_UI})
qt5_add_resources(RESOURCES_SOURCES resources.qrc)

set(ALL_SOURCES
    ${EDITOR_SOURCES}
    ${ASSETMANAGER_SOURCES}
    ${ASSETMANAGER_ACTIONS_SOURCES}
    ${ASSETMANAGER_ACTIONS_DELETE_SOURCES}
    ${ASSETMANAGER_ACTIONS_IMPORT_SOURCES}
    ${ASSETMANAGER_ACTIONS_NEW_SOURCES}
    ${ASSETMANAGER_ACTIONS_NEWFOLDER_SOURCES}
    ${ASSETMANAGER_ACTIONS_RENAME_SOURCES}
    ${ASSETMANAGER_CONTEXTMENU_SOURCES}
    ${ASSETMANAGER_MODEL_SOURCES}
    ${ASSETMODEL_SOURCES}
    ${ASSETMODEL_SYNC_SOURCES}
    ${COMPONENTS_SOURCES}
    ${COMPONENTS_NODEGRAPH_SOURCES}
    ${COMPONENTS_PREVIEW_SOURCES}
    ${EDITORS_STATICMESHEDITOR_SOURCES}
    ${EVENTS_SOURCES}
    ${LOADERS_SOURCES}
    ${PROJECT_SOURCES}
    )

add_library(Editor SHARED
    ${ALL_SOURCES}
    ${ALL_UI_SOURCES}
    ${RESOURCES_SOURCES}
    )

target_link_libraries(Editor
    Qt5::Core
    Qt5::Widgets
    Qt5::OpenGL
    Qt5::Xml
    CobaltSKY
    CobaltSKY-GraphicsGL4
    CobaltSKY-PhysicsBullet
    ${assimp_LIBRARIES}
    )

CS_INSTALL_MOCCER(Editor editor EDITOR_API ALL_SOURCES)

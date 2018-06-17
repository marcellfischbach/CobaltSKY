
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

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CobaltSKY_SOURCE_DIR}/Tools/Editor)
include_directories(${CobaltSKY_BINARY_DIR}/Tools/Editor)
set(CMAKE_AUTOMOC ON)

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
    main.cc
    editor.cc
    editor.hh
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
    idockitem.hh
    mainwindow.cc
    mainwindow.hh
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
    assetmanager/assetmanageraction.hh
    assetmanager/assetmanageractionmanager.cc
    assetmanager/assetmanageractionmanager.hh
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
    assetmanager/assetmanagerrenamer.cc
    assetmanager/assetmanagerrenamer.hh
    assetmanager/assetmanagerrenamehandler.cc
    assetmanager/assetmanagerrenamehandler.hh
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
    assetmanager/actions/assetmanagernewaction.cc
    assetmanager/actions/assetmanagernewaction.hh
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




set(ASSETMANAGER_IMPORT_SOURCES
    assetmanager/import/assetmanagerimporter.hh
    assetmanager/import/assetmanagerimporterdialog.cc
    assetmanager/import/assetmanagerimporterdialog.hh
    assetmanager/import/assetmanagerimporterpage.cc
    assetmanager/import/assetmanagerimporterpage.hh
    assetmanager/import/assetmanagerimporterlistmodel.cc
    assetmanager/import/assetmanagerimporterlistmodel.hh
    assetmanager/import/assetmanagerimportmanager.cc
    assetmanager/import/assetmanagerimportmanager.hh
    )

set(ASSETMANAGER_IMPORT_UI
    assetmanager/import/assetmanagerimporterdialog.ui
    assetmanager/import/assetmanagerimporterpage.ui
    )


source_group(
    "editor\\assetmanager\\import"
    FILES
    ${ASSETMANAGER_IMPORT_SOURCES}
    ${ASSETMANAGER_IMPORT_UI}
    )

set(ASSETMANAGER_MODEL_SOURCES
	assetmanager/model/assetlistmodel.cc
	assetmanager/model/assetlistmodel.hh
	assetmanager/model/foldertreemodel.cc
	assetmanager/model/foldertreemodel.hh
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
	assetmodel/folder.cc
	assetmodel/folder.hh
	assetmodel/model.cc
	assetmodel/model.hh
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
	assetmodel/sync/deletehandler.cc
	assetmodel/sync/deletehandler.hh
	assetmodel/sync/modelscanner.cc
	assetmodel/sync/modelscanner.hh
	assetmodel/sync/renamehandler.cc
	assetmodel/sync/renamehandler.hh
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


set(EDITORS_SAMPLEREDITOR_SOURCES
    editors/samplereditor/samplereditor.cc
    editors/samplereditor/samplereditor.hh
    editors/samplereditor/samplereditorfactory.cc
    editors/samplereditor/samplereditorfactory.hh
    editors/samplereditor/samplereditornewaction.cc
    editors/samplereditor/samplereditornewaction.hh
    editors/samplereditor/samplereditorwidget.cc
    editors/samplereditor/samplereditorwidget.hh
    )

set(EDITORS_SAMPLEREDITOR_UI
    editors/samplereditor/samplereditorwidget.ui
    )


source_group(
    "editor\\editors\\samplereditor"
    FILES
    ${EDITORS_SAMPLEREDITOR_SOURCES}
    ${EDITORS_SAMPLEREDITOR_UI}
    )



set(EDITORS_SHADERGRAPHEDITOR_SOURCES
    editors/shadergrapheditor/shadergrapheditor.cc
    editors/shadergrapheditor/shadergrapheditor.hh
    editors/shadergrapheditor/shadergrapheditorfactory.cc
    editors/shadergrapheditor/shadergrapheditorfactory.hh
    editors/shadergrapheditor/shadergrapheditormeta.cc
    editors/shadergrapheditor/shadergrapheditormeta.hh
    editors/shadergrapheditor/shadergrapheditormetaassetcsfloader.cc
    editors/shadergrapheditor/shadergrapheditormetaassetcsfloader.hh
    editors/shadergrapheditor/shadergrapheditormetaassetxmlloader.cc
    editors/shadergrapheditor/shadergrapheditormetaassetxmlloader.hh
    editors/shadergrapheditor/shadergrapheditornewaction.cc
    editors/shadergrapheditor/shadergrapheditornewaction.hh
    editors/shadergrapheditor/shadergrapheditornode.cc
    editors/shadergrapheditor/shadergrapheditornode.hh
    editors/shadergrapheditor/shadergrapheditoroutliner.cc
    editors/shadergrapheditor/shadergrapheditoroutliner.hh
    editors/shadergrapheditor/shadergrapheditoroutlinertablemodel.cc
    editors/shadergrapheditor/shadergrapheditoroutlinertablemodel.hh
    editors/shadergrapheditor/shadergrapheditorpreview.cc
    editors/shadergrapheditor/shadergrapheditorpreview.hh
    editors/shadergrapheditor/shadergrapheditorpreviewsceneview.cc
    editors/shadergrapheditor/shadergrapheditorpreviewsceneview.hh
    editors/shadergrapheditor/shadergrapheditorproperties.cc
    editors/shadergrapheditor/shadergrapheditorproperties.hh
    editors/shadergrapheditor/shadergrapheditorshadergraphproperties.cc
    editors/shadergrapheditor/shadergrapheditorshadergraphproperties.hh
    editors/shadergrapheditor/shadergrapheditortoolbox.cc
    editors/shadergrapheditor/shadergrapheditortoolbox.hh
    editors/shadergrapheditor/shadergrapheditortoolboxmodel.cc
    editors/shadergrapheditor/shadergrapheditortoolboxmodel.hh
    editors/shadergrapheditor/shadergrapheditorwidget.cc
    editors/shadergrapheditor/shadergrapheditorwidget.hh
    )

set(EDITORS_SHADERGRAPHEDITOR_UI
    editors/shadergrapheditor/shadergrapheditoroutliner.ui
    editors/shadergrapheditor/shadergrapheditorpreview.ui
    editors/shadergrapheditor/shadergrapheditorshadergraphproperties.ui
    editors/shadergrapheditor/shadergrapheditortoolbox.ui
    editors/shadergrapheditor/shadergrapheditorwidget.ui
    )


source_group(
    "editor\\editors\\shadergrapheditor"
    FILES
    ${EDITORS_SHADERGRAPHEDITOR_SOURCES}
    ${EDITORS_SHADERGRAPHEDITOR_UI}
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


set(EDITORS_TEXTUREEDITOR_SOURCES
    editors/textureeditor/textureeditor.cc
    editors/textureeditor/textureeditor.hh
    editors/textureeditor/textureeditorfactory.cc
    editors/textureeditor/textureeditorfactory.hh
    editors/textureeditor/textureeditorproperties.cc
    editors/textureeditor/textureeditorproperties.hh
    editors/textureeditor/textureeditorrenderwidget.cc
    editors/textureeditor/textureeditorrenderwidget.hh
    editors/textureeditor/textureeditorwidget.cc
    editors/textureeditor/textureeditorwidget.hh
    )

set(EDITORS_TEXTUREEDITOR_UI
    editors/textureeditor/textureeditorwidget.ui
    )


source_group(
    "editor\\editors\\textureeditor"
    FILES
    ${EDITORS_TEXTUREEDITOR_SOURCES}
    ${EDITORS_TEXTUREEDITOR_UI}
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

                    
set(IMPORTERS_SOURCES
    importers/iimporter.hh
    importers/importerdialog.cc
    importers/importerdialog.hh
    importers/importerfileslistmodel.cc
    importers/importerfileslistmodel.hh
    importers/imageimporter.cc
    importers/imageimporter.hh
    importers/modelimporter.cc
    importers/modelimporter.hh
    importers/staticmeshmodelimporterdata.cc
    importers/staticmeshmodelimporterdata.hh
    )

set(IMPORTERS_UI
    importers/importerdialog.ui
    )

source_group(
    "editor\\importers"
    FILES
    ${IMPORTERS_SOURCES}
    ${IMPORTERS_UI}
    )

set(IMPORTERS_IMAGE_SOURCES
    importers/image/imageimporter.cc
    importers/image/imageimporter.hh
    )

set(IMPORTERS_IMAGE_UI
    importers/image/imageimporter.ui
    )

source_group(
    "editors\\importers\\image"
    FILES
    ${IMPORTERS_IMAGE_SOURCES}
    ${IMPORTERS_IMAGE_UI}
    )


set(IMPORTERS_MODEL_SOURCES
    importers/model/assimpscenescanner.cc
    importers/model/assimpscenescanner.hh
    importers/model/modelimporter.cc
    importers/model/modelimporter.hh
    importers/model/modelmeshexporter.cc
    importers/model/modelmeshexporter.hh
    )

set(IMPORTERS_MODEL_UI
    importers/model/modelimporter.ui
    )

source_group(
    "editors\\importers\\model"
    FILES
    ${IMPORTERS_MODEL_SOURCES}
    ${IMPORTERS_MODEL_UI}
    )


set(LOADERS_SOURCES
    loaders/loaderseditoriconassetcsfloader.cc
    loaders/loaderseditoriconassetcsfloader.hh
    loaders/loaderseditoriconassetxmlloader.cc
    loaders/loaderseditoriconassetxmlloader.hh
    )


source_group(
    "editor\\loaders"
    FILES
    ${LOADERS_SOURCES}
    )


set(EDITORS_MATERIALEDITOR_SOURCES
    editors/materialeditor/materialeditor.cc
    editors/materialeditor/materialeditor.hh
    editors/materialeditor/materialeditorfactory.cc
    editors/materialeditor/materialeditorfactory.hh
    editors/materialeditor/materialeditornewaction.cc
    editors/materialeditor/materialeditornewaction.hh
    editors/materialeditor/materialeditorpreviewsceneview.cc
    editors/materialeditor/materialeditorpreviewsceneview.hh
    editors/materialeditor/materialeditorproperties.cc
    editors/materialeditor/materialeditorproperties.hh
    editors/materialeditor/materialeditorwidget.cc
    editors/materialeditor/materialeditorwidget.hh
    )

set(EDITORS_MATERIALEDITOR_UI
    editors/materialeditor/materialeditorwidget.ui
    )

source_group(
    "editor\\editors\\materialeditor"
    FILES
    ${EDITORS_MATERIALEDITOR_SOURCES}
    ${EDITORS_MATERIALEDITOR_UI}
    )

set(NODEGRAPH_SOURCES
    nodegraph/nodegraphacceptevent.cc
    nodegraph/nodegraphacceptevent.hh
    nodegraph/nodegraphnode.cc
    nodegraph/nodegraphnode.hh
    nodegraph/nodegraphnodeanchor.cc
    nodegraph/nodegraphnodeanchor.hh
    nodegraph/nodegraphnodeheader.cc
    nodegraph/nodegraphnodeheader.hh
    nodegraph/nodegraphnodeimageproperty.cc
    nodegraph/nodegraphnodeimageproperty.hh
    nodegraph/nodegraphnodeproperty.cc
    nodegraph/nodegraphnodeproperty.hh
    nodegraph/nodegraphnodepropertytype.hh
    nodegraph/nodegraphnodevalueproperty.cc
    nodegraph/nodegraphnodevalueproperty.hh
    nodegraph/nodegraphvetoevent.cc
    nodegraph/nodegraphvetoevent.hh
    nodegraph/nodegraphwidget.cc
    nodegraph/nodegraphwidget.hh
    )

    

source_group(
    "editor\\nodegraph"
    FILES
    ${NODEGRAPH_SOURCES}
    )


                    
set(PREVIEW_SOURCES
    preview/previewcameraflowhandler.cc
    preview/previewcameraflowhandler.hh
    preview/previewcameraorbithandler.cc
    preview/previewcameraorbithandler.hh
    preview/previewlightorbithandler.cc
    preview/previewlightorbithandler.hh
    preview/previewsceneview.cc
    preview/previewsceneview.hh
    )


source_group(
    "editor\\preview"
    FILES
    ${PREVIEW_SOURCES}
    )


set(PROJECT_SOURCES
    project/project.cc
    project/project.hh
    project/projectassetreference.cc
    project/projectassetreference.hh
	project/projectmodel.cc
	project/projectmodel.hh
	project/projectmodelscanner.cc
	project/projectmodelscanner.hh
    project/projectreferencetree.cc
    project/projectreferencetree.hh
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
    ${ASSETMANAGER_ACTIONS_RENAME_UI}
    ${ASSETMANAGER_IMPORT_UI}
    ${COMPONENTS_UI}
    ${EDITORS_MATERIALEDITOR_UI}
    ${EDITORS_SAMPLEREDITOR_UI}
    ${EDITORS_SHADERGRAPHEDITOR_UI}
    ${EDITORS_STATICMESHEDITOR_UI}
    ${EDITORS_TEXTUREEDITOR_UI}
    ${IMPORTERS_UI}
    ${IMPORTERS_IMAGE_UI}
    ${IMPORTERS_MODEL_UI}
    )


qt5_wrap_ui(ALL_UI_SOURCES ${ALL_UI})
qt5_add_resources(RESOURCES_SOURCES resources.qrc)

set(ALL_SOURCES
    ${EDITOR_SOURCES}
    ${ASSETMANAGER_SOURCES}
    ${ASSETMANAGER_ACTIONS_SOURCES}
    ${ASSETMANAGER_ACTIONS_DELETE_SOURCES}
    ${ASSETMANAGER_ACTIONS_RENAME_SOURCES}
    ${ASSETMANAGER_IMPORT_SOURCES}
    ${ASSETMANAGER_MODEL_SOURCES}
    ${ASSETMODEL_SOURCES}
    ${ASSETMODEL_SYNC_SOURCES}
    ${COMPONENTS_SOURCES}
    ${EDITORS_MATERIALEDITOR_SOURCES}
    ${EDITORS_SAMPLEREDITOR_SOURCES}
    ${EDITORS_SHADERGRAPHEDITOR_SOURCES}
    ${EDITORS_STATICMESHEDITOR_SOURCES}
    ${EDITORS_TEXTUREEDITOR_SOURCES}
    ${EVENTS_SOURCES}
    ${IMPORTERS_SOURCES}
    ${IMPORTERS_IMAGE_SOURCES}
    ${IMPORTERS_MODEL_SOURCES}
    ${LOADERS_SOURCES}
    ${NODEGRAPH_SOURCES}
    ${PREVIEW_SOURCES}
    ${PROJECT_SOURCES}
    )

add_executable(
    Editor
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

CS_INSTALL_MOCCER(Editor NO_PREFIX NO_API ALL_SOURCES)

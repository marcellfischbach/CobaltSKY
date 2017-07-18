
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
find_package(Qt5Xml)
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
    dockitems.hh
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
    assetmanager/actions/assetmanagerrenameaction.cc
    assetmanager/actions/assetmanagerrenameaction.hh
    assetmanager/actions/assetmanagerrenamedialog.cc
    assetmanager/actions/assetmanagerrenamedialog.hh
    )

set(ASSETMANAGER_ACTIONS_UI
    assetmanager/actions/assetmanagerrenamedialog.ui
    )
    
source_group(
    "editor\\assetmanager\\actions"
    FILES
    ${ASSETMANAGER_ACTIONS_SOURCES}
    ${ASSETMANAGER_ACTIONS_UI}
    )





set(ASSETMANAGER_IMPORT_SOURCES
    assetmanager/import/assetmanagerimporter.hh
    assetmanager/import/assetmanagerimporterdialog.cc
    assetmanager/import/assetmanagerimporterdialog.hh
    assetmanager/import/assetmanagerimporterlistmodel.cc
    assetmanager/import/assetmanagerimporterlistmodel.hh
    assetmanager/import/assetmanagerimportmanager.cc
    assetmanager/import/assetmanagerimportmanager.hh
    )

set(ASSETMANAGER_IMPORT_UI
    assetmanager/import/assetmanagerimporterdialog.ui
    )


source_group(
    "editor\\assetmanager\\import"
    FILES
    ${ASSETMANAGER_IMPORT_SOURCES}
    ${ASSETMANAGER_IMPORT_UI}
    )


set(COMPONENTS_SOURCES
    components/assetresourcelineedit.cc
    components/assetresourcelineedit.hh
    components/assetresourcewidget.cc
    components/assetresourcewidget.hh
    components/qtcsonscreenrendertarget.cc
    components/qtcsonscreenrendertarget.hh
    components/sceneview.cc
    components/sceneview.hh
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

 set(EVENTS_SOURCES
    events/assetaddedevent.cc
    events/assetaddedevent.hh
    events/assetrenamedevent.cc
    events/assetrenamedevent.hh
    )


source_group(
    "editor\\events"
    FILES
    ${EVENTS_SOURCES}
    )

                    
set(IMPORTERS_SOURCES
    importers/imageimporter.cc
    importers/imageimporter.hh
    importers/modelimporter.cc
    importers/modelimporter.hh
    )


source_group(
    "editor\\importers"
    FILES
    ${IMPORTERS_SOURCES}
    )


set(LOADERS_SOURCES
    loaders/loaderseditoriconassetxmlloader.cc
    loaders/loaderseditoriconassetxmlloader.hh
    )


source_group(
    "editor\\loaders"
    FILES
    ${LOADERS_SOURCES}
    )


set(MATERIALEDITOR_SOURCES
    materialeditor/materialeditor.cc
    materialeditor/materialeditor.hh
    materialeditor/materialeditorfactory.cc
    materialeditor/materialeditorfactory.hh
    materialeditor/materialeditornewaction.cc
    materialeditor/materialeditornewaction.hh
    materialeditor/materialeditorpreviewsceneview.cc
    materialeditor/materialeditorpreviewsceneview.hh
    materialeditor/materialeditorproperties.cc
    materialeditor/materialeditorproperties.hh
    materialeditor/materialeditorwidget.cc
    materialeditor/materialeditorwidget.hh
    )

set(MATERIALEDITOR_UI
    materialeditor/materialeditorwidget.ui
    )

source_group(
    "editor\\materialeditor"
    FILES
    ${MATERIALEDITOR_SOURCES}
    ${MATERIALEDITOR_UI}
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
    project/projectreferencetree.cc
    project/projectreferencetree.hh
    )

source_group(
    "editor\\project"
    FILES
    ${PROJECT_SOURCES}
    )


set(SAMPLEREDITOR_SOURCES
    samplereditor/samplereditor.cc
    samplereditor/samplereditor.hh
    samplereditor/samplereditorfactory.cc
    samplereditor/samplereditorfactory.hh
    samplereditor/samplereditornewaction.cc
    samplereditor/samplereditornewaction.hh
    samplereditor/samplereditorwidget.cc
    samplereditor/samplereditorwidget.hh
    )

set(SAMPLEREDITOR_UI
    samplereditor/samplereditorwidget.ui
    )


source_group(
    "editor\\samplereditor"
    FILES
    ${SAMPLEREDITOR_SOURCES}
    ${SAMPLEREDITOR_UI}
    )

    

set(SHADERGRAPHEDITOR_SOURCES
    shadergrapheditor/shadergrapheditor.cc
    shadergrapheditor/shadergrapheditor.hh
    shadergrapheditor/shadergrapheditorfactory.cc
    shadergrapheditor/shadergrapheditorfactory.hh
    shadergrapheditor/shadergrapheditormeta.cc
    shadergrapheditor/shadergrapheditormeta.hh
    shadergrapheditor/shadergrapheditormetaassetxmlloader.cc
    shadergrapheditor/shadergrapheditormetaassetxmlloader.hh
    shadergrapheditor/shadergrapheditornewaction.cc
    shadergrapheditor/shadergrapheditornewaction.hh
    shadergrapheditor/shadergrapheditornode.cc
    shadergrapheditor/shadergrapheditornode.hh
    shadergrapheditor/shadergrapheditoroutliner.cc
    shadergrapheditor/shadergrapheditoroutliner.hh
    shadergrapheditor/shadergrapheditoroutlinertablemodel.cc
    shadergrapheditor/shadergrapheditoroutlinertablemodel.hh
    shadergrapheditor/shadergrapheditorpreview.cc
    shadergrapheditor/shadergrapheditorpreview.hh
    shadergrapheditor/shadergrapheditorpreviewsceneview.cc
    shadergrapheditor/shadergrapheditorpreviewsceneview.hh
    shadergrapheditor/shadergrapheditorproperties.cc
    shadergrapheditor/shadergrapheditorproperties.hh
    shadergrapheditor/shadergrapheditorshadergraphproperties.cc
    shadergrapheditor/shadergrapheditorshadergraphproperties.hh
    shadergrapheditor/shadergrapheditortoolbox.cc
    shadergrapheditor/shadergrapheditortoolbox.hh
    shadergrapheditor/shadergrapheditortoolboxmodel.cc
    shadergrapheditor/shadergrapheditortoolboxmodel.hh
    shadergrapheditor/shadergrapheditorwidget.cc
    shadergrapheditor/shadergrapheditorwidget.hh
    )

set(SHADERGRAPHEDITOR_UI
    shadergrapheditor/shadergrapheditoroutliner.ui
    shadergrapheditor/shadergrapheditorpreview.ui
    shadergrapheditor/shadergrapheditorshadergraphproperties.ui
    shadergrapheditor/shadergrapheditortoolbox.ui
    shadergrapheditor/shadergrapheditorwidget.ui
    )


source_group(
    "editor\\shadergrapheditor"
    FILES
    ${SHADERGRAPHEDITOR_SOURCES}
    ${SHADERGRAPHEDITOR_UI}
    )

    
set(STATICMESHEDITOR_SOURCES
    staticmesheditor/staticmesheditor.cc
    staticmesheditor/staticmesheditor.hh
    staticmesheditor/staticmesheditorfactory.cc
    staticmesheditor/staticmesheditorfactory.hh
    staticmesheditor/staticmesheditorpreviewsceneview.cc
    staticmesheditor/staticmesheditorpreviewsceneview.hh
    staticmesheditor/staticmesheditorwidget.cc
    staticmesheditor/staticmesheditorwidget.hh
    )

set(STATICMESHEDITOR_UI
    staticmesheditor/staticmesheditorwidget.ui
    )


source_group(
    "editor\\staticmesheditor"
    FILES
    ${STATICMESHEDITOR_SOURCES}
    ${STATICMESHEDITOR_UI}
    )

    
set(TEXTUREEDITOR_SOURCES
    textureeditor/textureeditor.cc
    textureeditor/textureeditor.hh
    textureeditor/textureeditorfactory.cc
    textureeditor/textureeditorfactory.hh
    textureeditor/textureeditorproperties.cc
    textureeditor/textureeditorproperties.hh
    textureeditor/textureeditorrenderwidget.cc
    textureeditor/textureeditorrenderwidget.hh
    textureeditor/textureeditorwidget.cc
    textureeditor/textureeditorwidget.hh
    )

set(TEXTUREEDITOR_UI
    textureeditor/textureeditorwidget.ui
    )


source_group(
    "editor\\textureeditor"
    FILES
    ${TEXTUREEDITOR_SOURCES}
    ${TEXTUREEDITOR_UI}
    )

set(ALL_UI
    ${EDITOR_UI}
    ${ASSETMANAGER_UI}
    ${ASSETMANAGER_ACTIONS_UI}
    ${ASSETMANAGER_IMPORT_UI}
    ${COMPONENTS_UI}
    ${MATERIALEDITOR_UI}
    ${SAMPLEREDITOR_UI}
    ${SHADERGRAPHEDITOR_UI}
    ${STATICMESHEDITOR_UI}
    ${TEXTUREEDITOR_UI}
    )


qt5_wrap_ui(ALL_UI_SOURCES ${ALL_UI})
qt5_add_resources(RESOURCES_SOURCES resources.qrc)

set(ALL_SOURCES
    ${EDITOR_SOURCES}
    ${ASSETMANAGER_SOURCES}
    ${ASSETMANAGER_ACTIONS_SOURCES}
    ${ASSETMANAGER_IMPORT_SOURCES}
    ${COMPONENTS_SOURCES}
    ${EVENTS_SOURCES}
    ${IMPORTERS_SOURCES}
    ${LOADERS_SOURCES}
    ${MATERIALEDITOR_SOURCES}
    ${NODEGRAPH_SOURCES}
    ${PREVIEW_SOURCES}
    ${PROJECT_SOURCES}
    ${SAMPLEREDITOR_SOURCES}
    ${SHADERGRAPHEDITOR_SOURCES}
    ${STATICMESHEDITOR_SOURCES}
    ${TEXTUREEDITOR_SOURCES}
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

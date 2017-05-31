
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
    glcontext.cc
    glcontext.hh
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
    mainwindow.ui
    )

source_group(
    "editor"
    FILES
    ${EDITOR_SOURCES}
    ${EDITOR_UI}
    )

set(ASSETMANAGER_SOURCES
    assetmanager/assetmanagercontentmodel.cc
    assetmanager/assetmanagercontentmodel.hh
    assetmanager/assetmanagerdock.cc
    assetmanager/assetmanagerdock.hh
    assetmanager/assetmanagerfoldermodel.cc
    assetmanager/assetmanagerfoldermodel.hh
    assetmanager/assetmanagernewhandler.hh
    assetmanager/assetmanagernewmanager.cc
    assetmanager/assetmanagernewmanager.hh
    assetmanager/assetmanagerresourcescanner.cc
    assetmanager/assetmanagerresourcescanner.hh
    assetmanager/assetmanagerwidget.cc
    assetmanager/assetmanagerwidget.hh
    )

set(ASSETMANAGER_UI
    assetmanager/assetmanagerwidget.ui
    )

source_group(
    "editor\\assetmanager"
    FILES
    ${ASSETMANAGER_SOURCES}
    ${ASSETMANAGER_UI}
    )

set(COMPONENTS_SOURCES
    components/assetresourcelineedit.cc
    components/assetresourcelineedit.hh
    components/assetresourcewidget.cc
    components/assetresourcewidget.hh
    components/qtvkonscreenrendertarget.cc
    components/qtvkonscreenrendertarget.hh
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

set(LOADERS_SOURCES
    loaders/loaderseditoriconassetxmlloader.cc
    loaders/loaderseditoriconassetxmlloader.hh
    )


source_group(
    "editor\\loaders"
    FILES
    ${LOADERS_SOURCES}
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
    project/projectdependencytree.cc
    project/projectdependencytree.hh
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
    shadergrapheditor/shadergrapheditornewhandler.cc
    shadergrapheditor/shadergrapheditornewhandler.hh
    shadergrapheditor/shadergrapheditornode.cc
    shadergrapheditor/shadergrapheditornode.hh
    shadergrapheditor/shadergrapheditornewhandler.cc
    shadergrapheditor/shadergrapheditornewhandler.hh
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
    ${COMPONENTS_UI}
    ${SAMPLEREDITOR_UI}
    ${SHADERGRAPHEDITOR_UI}
    ${TEXTUREEDITOR_UI}
    )

qt5_wrap_ui(ALL_UI_SOURCES ${ALL_UI})
qt5_add_resources(RESOURCES_SOURCES resources.qrc)

set(ALL_SOURCES
    ${EDITOR_SOURCES}
    ${ASSETMANAGER_SOURCES}
    ${COMPONENTS_SOURCES}
    ${LOADERS_SOURCES}
    ${NODEGRAPH_SOURCES}
    ${PREVIEW_SOURCES}
    ${PROJECT_SOURCES}
    ${SAMPLEREDITOR_SOURCES}
    ${SHADERGRAPHEDITOR_SOURCES}
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
    ValkyrieEngine
    ValkyrieEngine-GraphicsGL4
    ValkyrieEngine-PhysicsBullet
    ${ASSIMP_LIBRARY}
    )

VK_INSTALL_MOCCER(Editor NO_PREFIX NO_API ALL_SOURCES)

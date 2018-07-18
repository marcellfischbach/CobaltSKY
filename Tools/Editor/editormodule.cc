

#include <editormodule.hh>
#include <master.refl.cc>

#include <cobalt/core/csresourcemanager.hh>
#include <editor.hh>
#include <assetmanager/assetmanagerrenamehandler.hh>
#include <assetmanager/actions/rename/assetmanagerrenameaction.hh>
#include <assetmanager/actions/newfolder/assetmanagernewfolderaction.hh>
#include <assetmanager/actions/delete/assetmanagerdeleteaction.hh>
#include <assetmanager/contextmenu/contextmenuhandler.hh>

#include <assetmanager/import/assetmanagerimportmanager.hh>
#include <editors/materialeditor/materialeditorfactory.hh>
#include <editors/materialeditor/materialeditornew.hh>
#include <editors/samplereditor/samplereditorfactory.hh>
#include <editors/samplereditor/samplereditornew.hh>
#include <editors/shadergrapheditor/shadergrapheditorfactory.hh>
#include <editors/shadergrapheditor/shadergrapheditornew.hh>
#include <editors/shadergrapheditor/shadergrapheditormetaassetcsfloader.hh>
#include <editors/shadergrapheditor/shadergrapheditormetaassetxmlloader.hh>
#include <editors/staticmesheditor/staticmesheditorfactory.hh>
#include <editors/textureeditor/textureeditorfactory.hh>

#include <importers/imageimporter.hh>
#include <importers/modelimporter.hh>
#include <importers/image/imageimporter.hh>
#include <importers/model/modelimporter.hh>

#include <loaders/loaderseditoriconassetxmlloader.hh>

void EditorModule::Initialize()
{
	register_classes();


	Editor *editor = Editor::Get();
	editor->AddEditorFactory(new MaterialEditorFactory());
	editor->AddEditorFactory(new SamplerEditorFactory());
	editor->AddEditorFactory(new ShaderGraphEditorFactory());
	editor->AddEditorFactory(new StaticMeshEditorFactory());
	editor->AddEditorFactory(new TextureEditorFactory());

	editor->AddImporterFactory(new ImageImporterFactory());
	editor->AddImporterFactory(new ModelImporterFactory());

	asset::contextmenu::Handler *contextMenu = asset::contextmenu::Handler::Get();
	contextMenu->AddFactory(new asset::actions::AssetManagerRenameContextMenu());
	contextMenu->AddFactory(new asset::actions::AssetManagerNewFolderContextMenu());
	contextMenu->AddFactory(new asset::actions::AssetManagerDeleteContextMenu());
	contextMenu->AddFactory(new MaterialEditorContextMenuFactory());
	contextMenu->AddFactory(new SamplerEditorContextMenuFactory());
	contextMenu->AddFactory(new ShaderGraphEditorContextMenuFactory());

	AssetManagerImportManager *importManager = AssetManagerImportManager::Get();
	importManager->RegisterImporter(new ImageImporter());
	importManager->RegisterImporter(new ModelImporter());

	csResourceManager *mgr = csResourceManager::Get();
	mgr->RegisterLoader(new ShaderGraphEditorMetaAssetCSFLoader());
	mgr->RegisterLoader(new ShaderGraphEditorMetaAssetXMLLoader());
	mgr->RegisterLoader(new LoadersEditorIconAssetCSFLoader());
	mgr->RegisterLoader(new LoadersEditorIconAssetXMLLoader());


	AssetManagerRenameHandler::Register();
}

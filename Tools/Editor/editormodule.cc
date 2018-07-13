

#include <editormodule.hh>
#include <master.refl.cc>

#include <cobalt/core/csresourcemanager.hh>
#include <editor.hh>
#include <assetmanager/assetmanageractionmanager.hh>
#include <assetmanager/assetmanagerrenamehandler.hh>
#include <assetmanager/actions/rename/assetmanagerrenameaction.hh>
#include <assetmanager/actions/delete/assetmanagerdeleteaction.hh>
#include <assetmanager/contextmenu/contextmenuhandler.hh>

#include <assetmanager/import/assetmanagerimportmanager.hh>
#include <editors/materialeditor/materialeditorcontextmenufactory.hh>
#include <editors/materialeditor/materialeditornewaction.hh>
#include <editors/materialeditor/materialeditorfactory.hh>
#include <editors/samplereditor/samplereditorfactory.hh>
#include <editors/samplereditor/samplereditornew.hh>
#include <editors/shadergrapheditor/shadergrapheditornew.hh>
#include <editors/shadergrapheditor/shadergrapheditornewaction.hh>
#include <editors/shadergrapheditor/shadergrapheditorfactory.hh>
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
	contextMenu->AddFactory(new MaterialEditorContextMenuFactory());
	contextMenu->AddFactory(new SamplerEditorContextMenuFactory());
	contextMenu->AddFactory(new ShaderGraphEditorContextMenuFactory());

	AssetManagerActionManager *actionManager = AssetManagerActionManager::Get();
	actionManager->RegisterNewAction(new MaterialEditorNewAction());
	actionManager->RegisterNewAction(new ShaderGraphEditorNewAction());
	actionManager->RegisterAction(new AssetManagerDeleteAction());

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



#include <editor/editormodule.hh>
#include <editor/master.refl.cc>

#include <cobalt/core/csresourcemanager.hh>
#include <editor/editor.hh>
#include <editor/assetmanager/actions/rename/assetmanagerrenameaction.hh>
#include <editor/assetmanager/actions/newfolder/assetmanagernewfolderaction.hh>
#include <editor/assetmanager/actions/delete/assetmanagerdeleteaction.hh>
#include <editor/assetmanager/contextmenu/contextmenuhandler.hh>

#include <editor/assetmanager/import/assetmanagerimportmanager.hh>
/*
#include <editor/editors/materialeditor/materialeditorfactory.hh>
#include <editor/editors/materialeditor/materialeditornew.hh>
#include <editor/editors/samplereditor/samplereditorfactory.hh>
#include <editor/editors/samplereditor/samplereditornew.hh>
#include <editor/editors/shadergrapheditor/shadergrapheditorfactory.hh>
#include <editor/editors/shadergrapheditor/shadergrapheditornew.hh>
#include <editor/editors/shadergrapheditor/shadergrapheditormetaassetcsfloader.hh>
#include <editor/editors/staticmesheditor/staticmesheditorfactory.hh>
#include <editor/editors/textureeditor/textureeditorfactory.hh>
*/

#include <editor/importers/imageimporter.hh>
#include <editor/importers/modelimporter.hh>
#include <editor/importers/image/imageimporter.hh>
#include <editor/importers/model/modelimporter.hh>


void EditorModule::Initialize()
{
	register_classes();


	Editor *editor = Editor::Get();
  /*
	editor->AddEditorFactory(new MaterialEditorFactory());
	editor->AddEditorFactory(new SamplerEditorFactory());
	editor->AddEditorFactory(new ShaderGraphEditorFactory());
	editor->AddEditorFactory(new StaticMeshEditorFactory());
	editor->AddEditorFactory(new TextureEditorFactory());
  */
	editor->AddImporterFactory(new ImageImporterFactory());
	editor->AddImporterFactory(new ModelImporterFactory());

	asset::contextmenu::Handler *contextMenu = asset::contextmenu::Handler::Get();
	contextMenu->AddFactory(new asset::actions::AssetManagerRenameContextMenu());
	contextMenu->AddFactory(new asset::actions::AssetManagerNewFolderContextMenu());
	contextMenu->AddFactory(new asset::actions::AssetManagerDeleteContextMenu());
	//contextMenu->AddFactory(new MaterialEditorContextMenuFactory());
	//contextMenu->AddFactory(new SamplerEditorContextMenuFactory());
	//contextMenu->AddFactory(new ShaderGraphEditorContextMenuFactory());

	AssetManagerImportManager *importManager = AssetManagerImportManager::Get();
	importManager->RegisterImporter(new ImageImporter());
	importManager->RegisterImporter(new ModelImporter());

	csResourceManager *mgr = csResourceManager::Get();
	//mgr->RegisterLoader(new ShaderGraphEditorMetaAssetCSFLoader());
	mgr->RegisterLoader(new LoadersEditorIconAssetCSFLoader());


}

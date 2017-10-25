

#include <editormodule.hh>
#include <master.refl.cc>

#include <cobalt/core/csresourcemanager.hh>
#include <editor.hh>
#include <assetmanager/assetmanageractionmanager.hh>
#include <assetmanager/assetmanagerrenamehandler.hh>
#include <assetmanager/actions/assetmanagerrenameaction.hh>
#include <assetmanager/import/assetmanagerimportmanager.hh>
#include <materialeditor/materialeditornewaction.hh>
#include <materialeditor/materialeditorfactory.hh>
#include <samplereditor/samplereditorfactory.hh>
#include <samplereditor/samplereditornewaction.hh>
#include <shadergrapheditor/shadergrapheditornewaction.hh>
#include <shadergrapheditor/shadergrapheditorfactory.hh>
#include <shadergrapheditor/shadergrapheditormetaassetcsfloader.hh>
#include <shadergrapheditor/shadergrapheditormetaassetxmlloader.hh>
#include <staticmesheditor/staticmesheditorfactory.hh>
#include <textureeditor/textureeditorfactory.hh>

#include <importers/imageimporter.hh>
#include <importers/modelimporter.hh>

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

  AssetManagerActionManager *actionManager = AssetManagerActionManager::Get();
  actionManager->RegisterNewAction(new MaterialEditorNewAction());
  actionManager->RegisterNewAction(new ShaderGraphEditorNewAction());
  actionManager->RegisterNewAction(new SamplerEditorNewAction());
  actionManager->RegisterAction(new AssetManagerRenameAction());

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

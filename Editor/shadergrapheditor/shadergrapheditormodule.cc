
#include <shadergrapheditor/shadergrapheditormodule.hh>
#include <shadergrapheditor/shadergrapheditorfactory.hh>
#include <shadergrapheditor/shadergrapheditornew.hh>
#include <shadergrapheditor/master.refl.cc>
#include <editor/editorfactorymanager.hh>
#include <editor/assetmanager/contextmenu/contextmenuhandler.hh>

void ShaderGraphEditorModule::Initialize(int argc, char **argv)
{
  register_classes();

  EditorFactoryManager *editorFactoryManager = EditorFactoryManager::Get();
  editorFactoryManager->AddEditorFactory(new ShaderGraphEditorFactory());


  asset::contextmenu::Handler *contextMenu = asset::contextmenu::Handler::Get();
  contextMenu->AddFactory(new ShaderGraphEditorContextMenuFactory());

  csResourceManager *mgr = csResourceManager::Get();
  mgr->RegisterLoader(new ShaderGraphEditorMetaAssetCSFLoader());

}


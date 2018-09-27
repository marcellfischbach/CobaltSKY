
#include <samplereditor/samplereditormodule.hh>
#include <samplereditor/samplereditorfactory.hh>
#include <samplereditor/samplereditornew.hh>
#include <samplereditor/master.refl.cc>
#include <editor/editorfactorymanager.hh>
#include <editor/assetmanager/contextmenu/contextmenuhandler.hh>

void SamplerEditorModule::Initialize(int argc, char **argv)
{
  register_classes();

  EditorFactoryManager *editorFactoryManager = EditorFactoryManager::Get();
  editorFactoryManager->AddEditorFactory(new SamplerEditorFactory());


  asset::contextmenu::Handler *contextMenu = asset::contextmenu::Handler::Get();
  contextMenu->AddFactory(new SamplerEditorContextMenuFactory());

}
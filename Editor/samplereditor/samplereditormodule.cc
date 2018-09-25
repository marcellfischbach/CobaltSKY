
#include <samplereditor/samplereditormodule.hh>
#include <samplereditor/samplereditorfactory.hh>
#include <samplereditor/samplereditornew.hh>
#include <samplereditor/master.refl.cc>
#include <editor/editor.hh>
#include <editor/assetmanager/contextmenu/contextmenuhandler.hh>

void SamplerEditorModule::Initialize(int argc, char **argv)
{
  register_classes();

  Editor *editor = Editor::Get();
  editor->AddEditorFactory(new SamplerEditorFactory());


  asset::contextmenu::Handler *contextMenu = asset::contextmenu::Handler::Get();
  contextMenu->AddFactory(new SamplerEditorContextMenuFactory());

}
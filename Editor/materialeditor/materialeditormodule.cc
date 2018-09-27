
#include <materialeditor/materialeditormodule.hh>
#include <materialeditor/materialeditorfactory.hh>
#include <materialeditor/materialeditornew.hh>
#include <materialeditor/master.refl.cc>
#include <editor/editorfactorymanager.hh>
#include <editor/assetmanager/contextmenu/contextmenuhandler.hh>

void MaterialEditorModule::Initialize(int argc, char **argv)
{
  register_classes();

  EditorFactoryManager *editorFactoryManager = EditorFactoryManager::Get();
  editorFactoryManager->AddEditorFactory(new MaterialEditorFactory());


  asset::contextmenu::Handler *contextMenu = asset::contextmenu::Handler::Get();
  contextMenu->AddFactory(new MaterialEditorContextMenuFactory());

}
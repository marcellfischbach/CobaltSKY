
#include <textureeditor/textureeditormodule.hh>
#include <textureeditor/textureeditorfactory.hh>
#include <textureeditor/master.refl.cc>
#include <editor/editorfactorymanager.hh>
#include <editor/assetmanager/contextmenu/contextmenuhandler.hh>

void TextureEditorModule::Initialize(int argc, char **argv)
{
  register_classes();

  EditorFactoryManager *editorFactoryManager = EditorFactoryManager::Get();
  editorFactoryManager->AddEditorFactory(new TextureEditorFactory());



}
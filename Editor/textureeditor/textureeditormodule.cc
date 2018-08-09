
#include <textureeditor/textureeditormodule.hh>
#include <textureeditor/textureeditorfactory.hh>
#include <textureeditor/master.refl.cc>
#include <editor/editor.hh>
#include <editor/assetmanager/contextmenu/contextmenuhandler.hh>

void TextureEditorModule::Initialize(int argc, char **argv)
{
  register_classes();

  Editor *editor = Editor::Get();
  editor->AddEditorFactory(new TextureEditorFactory());



}
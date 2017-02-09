

#include <editormodule.hh>
#include <master.refl.cc>

#include <editor.hh>
#include <samplereditor/samplereditorfactory.hh>
#include <shadergrapheditor/shadergrapheditorfactory.hh>
#include <textureeditor/textureeditorfactory.hh>

void EditorModule::Initialize()
{
  register_classes();


  Editor *editor = Editor::Get();
  editor->AddEditorFactory(new SamplerEditorFactory());
  editor->AddEditorFactory(new ShaderGraphEditorFactory());
  editor->AddEditorFactory(new TextureEditorFactory());
}
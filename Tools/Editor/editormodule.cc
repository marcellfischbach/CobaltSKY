

#include <editormodule.hh>
#include <master.refl.cc>

#include <editor.hh>
#include <samplereditor/samplereditorfactory.hh>

void EditorModule::Initialize()
{
  register_classes();


  Editor *editor = Editor::Get();
  editor->AddEditorFactory(new SamplerEditorFactory());
}
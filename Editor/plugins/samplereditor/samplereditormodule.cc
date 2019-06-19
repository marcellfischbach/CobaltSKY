

#include <plugins/samplereditor/samplereditormodule.hh>
#include <plugins/samplereditor/samplereditorfactory.hh>
#include <editorcore/editor.hh>

#include <plugins/samplereditor/master.refl.cc>

namespace cs::editor::plugin::samplereditor
{


bool Module::Initialize(int argc, char** argv)
{
  register_classes();

  cs::editor::core::Editor::Get()->Register(new SamplerEditorFactory());
  return true;
}


}
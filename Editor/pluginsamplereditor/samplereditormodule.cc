

#include <pluginsamplereditor/samplereditormodule.hh>
#include <pluginsamplereditor/samplereditorfactory.hh>
#include <editorcore/editor.hh>

namespace cs::editor::plugin::samplereditor
{


bool Module::Initialize(int argc, char** argv)
{
  cs::editor::core::Editor::Get()->Register(new SamplerEditorFactory());
  return true;
}


}
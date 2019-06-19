

#include <plugins/textureeditor/textureeditormodule.hh>
#include <plugins/textureeditor/textureeditorfactory.hh>
#include <editorcore/editor.hh>

#include <plugins/textureeditor/master.refl.cc>

namespace cs::editor::plugin::textureeditor
{


bool Module::Initialize(int argc, char** argv)
{
  register_classes();

  cs::editor::core::Editor::Get()->Register(new TextureEditorFactory());
  return true;
}


}
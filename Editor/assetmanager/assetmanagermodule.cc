

#include <assetmanager/assetmanagermodule.hh>
#include <assetmanager/assetmanagertoolview.hh>
#include <editorcore/editor.hh>
#include <editorcore/editorwindow.hh>

namespace cs::editor::assetmanager
{


bool Module::Initialize(int argc, char** argv)
{
  cs::editor::core::Editor::Get()->GetEditorWindow()->Add(new AssetManagerToolView());
  return true;
}


}
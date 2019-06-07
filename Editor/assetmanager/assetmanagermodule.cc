

#include <assetmanager/assetmanagermodule.hh>
#include <assetmanager/assetmanagertoolview.hh>
#include <assetmanager/menu/menuitemfactories.hh>
#include <assetmanager/menu/items/delete/deletemenuitem.hh>
#include <assetmanager/menu/items/newfolder/newfoldermenuitem.hh>
#include <assetmanager/menu/items/rename/renamemenuitem.hh>

#include <editorcore/editor.hh>
#include <editorcore/editorwindow.hh>

namespace cs::editor::assetmanager
{


bool Module::Initialize(int argc, char** argv)
{
  cs::editor::core::Editor::Get()->GetEditorWindow()->Add(new AssetManagerToolView());

  cs::editor::assetmanager::MenuItemFactories& factories = cs::editor::assetmanager::MenuItemFactories::Get();
  factories.AddFactory(new cs::editor::assetmanager::NewFolderMenuItemFactory());
  factories.AddFactory(new cs::editor::assetmanager::DeleteMenuItemFactory());
  factories.AddFactory(new cs::editor::assetmanager::RenameMenuItemFactory());

  return true;
}


}

#include <assetmanager/actions/assetmanagernewaction.hh>
#include <editor.hh>
#include <project/project.hh>
#include <project/projectreferencetree.hh>


AssetManagerNewAction::AssetManagerNewAction()
  :AssetManagerAction()
{

}

AssetManagerNewAction::~AssetManagerNewAction()
{

}

bool AssetManagerNewAction::PerformAction(AssetManagerWidget *widget) const
{
  csResourceLocator locator = CreateNewAsset(widget);
  if (locator.IsValid())
  {
    Editor::Get()->GetProject()->GetReferenceTree().UpdateDependencyTree(locator);
    return true;
  }
  return false;
}
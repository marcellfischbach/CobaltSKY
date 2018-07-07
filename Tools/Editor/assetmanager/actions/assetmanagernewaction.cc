
#include <assetmanager/actions/assetmanagernewaction.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>
#include <assetmodel/modeltransaction.hh>
#include <editor.hh>
#include <project/project.hh>
#include <project/projectreferencetree.hh>


AssetManagerNewAction::AssetManagerNewAction(const std::string &baseName)
  :AssetManagerAction()
  , m_baseName(baseName)
{

}

AssetManagerNewAction::~AssetManagerNewAction()
{

}

bool AssetManagerNewAction::PerformAction(AssetManagerWidget *widget) const
{
  asset::model::Folder *folder = widget->GetCurrentFolder();
  if (!folder)
  {
    printf("AssetManagerNewAction: No current folder\n");
    return false;
  }
  if (m_baseName.empty())
  {
    printf("AssetManagerNewAction: No base name\n");
    return false;
  }
  std::string fileName = folder->GetNewAssetName(m_baseName);
  if (fileName.empty())
  {
    printf("AssetManagerNewAction: Cannot create a new asset name\n");
    return false;
  }
  csResourceLocator locator = folder->GetResourceLocator().WithFileSuffix(fileName);
  printf("CreateAsset: %s\n", locator.GetText().c_str());
  if (!locator.IsValid())
  {
    return false;
  }
  if (!CreateNewAsset(folder, locator, fileName))
  {
    return false;
  }

  asset::model::Asset *asset = folder->GetModel()->CreateAsset(fileName);
  if (!asset)
  {
    return false;
  }
  asset::model::ModelTransaction tr;
  try
  {
    folder->Add(asset, tr);
    tr.Commit();
  }
  catch (const std::exception &e)
  {
    tr.Rollback();
  }
  return true;
  /*
  csResourceLocator locator = CreateNewAsset(widget);
  if (locator.IsValid())
  {
    Editor::Get()->GetProject()->GetReferenceTree().UpdateDependencyTree(locator);
    return true;
  }
  return false;
  */
}

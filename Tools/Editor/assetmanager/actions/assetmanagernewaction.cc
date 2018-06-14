
#include <assetmanager/actions/assetmanagernewaction.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>
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
		return false;
	}
	if (m_baseName.empty())
	{
		return false;
	}
	std::string fileName = folder->GetNewAssetName(m_baseName);
	if (fileName.empty())
	{
		return false;
	}
	csResourceLocator locator = folder->GetResourceLocator().WithFileSuffix(fileName);
	if (!locator.IsValid())
	{
		return false;
	}
	printf("CreateAsset\n");
	if (!CreateNewAsset(folder, locator, fileName))
	{
		return false;
	}

	asset::model::Asset *asset = folder->GetModel()->CreateAsset(fileName);
	if (!asset)
	{
		return false;
	}
	folder->Add(asset);
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

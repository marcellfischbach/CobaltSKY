

#include <assetmanager/assetmanageractionmanager.hh>
#include <assetmanager/assetmanageraction.hh>
#include <assetmanager/actions/assetmanagernewaction.hh>

AssetManagerActionManager::AssetManagerActionManager()
{
}

AssetManagerActionManager *AssetManagerActionManager::Get()
{
  static AssetManagerActionManager static_instance;
  return &static_instance;
}

void AssetManagerActionManager::RegisterAction(AssetManagerAction *action)
{
  m_actions.push_back(action);
}

void AssetManagerActionManager::RegisterNewAction(AssetManagerNewAction *action)
{
  m_newActions.push_back(action);
}


const std::vector<AssetManagerAction*> &AssetManagerActionManager::GetActions() const
{
  return m_actions;
}

const std::vector<AssetManagerNewAction*> &AssetManagerActionManager::GetNewActions() const
{
  return m_newActions;
}

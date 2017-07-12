#pragma once

#include <vector>

struct AssetManagerAction;
class AssetManagerNewAction;
class AssetManagerActionManager
{
public:
  static AssetManagerActionManager *Get();

  void RegisterAction(AssetManagerAction *action);
  void RegisterNewAction(AssetManagerNewAction *action);

  const std::vector<AssetManagerAction*> &GetActions() const;
  const std::vector<AssetManagerNewAction*> &GetNewActions() const;

private:
  AssetManagerActionManager();

  std::vector<AssetManagerAction*> m_actions;
  std::vector<AssetManagerNewAction*> m_newActions;
};
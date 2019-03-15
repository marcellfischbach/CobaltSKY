
#pragma once

#include <assetmanager/assetmanagerexport.hh>
#include <vector>


namespace cs::editor::assetmanager
{

class IMenuItemFactory;
class ASSET_MANAGER_API MenuItemFactories
{
public:
  static MenuItemFactories &Get();

  void AddFactory(IMenuItemFactory *factory);
  const std::vector<IMenuItemFactory*> &GetFactories() const;

private:
  MenuItemFactories();

  std::vector<IMenuItemFactory*> m_factories;


};

}
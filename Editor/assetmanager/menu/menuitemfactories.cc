
#include <assetmanager/menu/menuitemfactories.hh>
#include <assetmanager/menu/imenuitemfactory.hh>

namespace cs::editor::assetmanager
{

MenuItemFactories::MenuItemFactories()
{

}

MenuItemFactories &MenuItemFactories::Get()
{
  static MenuItemFactories factories;
  return factories;
}


void MenuItemFactories::AddFactory(IMenuItemFactory *factory)
{
  m_factories.push_back(factory);
}

const std::vector<IMenuItemFactory*> &MenuItemFactories::GetFactories() const
{
  return m_factories;
}

}
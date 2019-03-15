

#pragma once

#include <assetmanager/assetmanagerexport.hh>
#include <string>


namespace cs::editor::assetmanager
{


class ASSET_MANAGER_API MenuItemDeclaration
{
public:
  MenuItemDeclaration(const std::string &menuName);

  const std::string &GetMenuName() const;

  static MenuItemDeclaration New();
  static MenuItemDeclaration Edit();

private:
  std::string m_menuName;
};


}
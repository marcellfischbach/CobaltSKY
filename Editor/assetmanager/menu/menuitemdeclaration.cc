

#include <assetmanager/menu/menuitemdeclaration.hh>
#include <QObject>


namespace cs::editor::assetmanager
{

MenuItemDeclaration::MenuItemDeclaration(const std::string &menuName)
  : m_menuName(menuName)
{

}

const std::string &MenuItemDeclaration::GetMenuName() const
{
  return m_menuName;
}


MenuItemDeclaration MenuItemDeclaration::New()
{
  return MenuItemDeclaration(QObject::tr("New").toLatin1().data());
}

MenuItemDeclaration MenuItemDeclaration::Edit()
{
  return MenuItemDeclaration(QObject::tr("Edit").toLatin1().data());
}


}
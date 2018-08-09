#include <AssetManager/NewAssetAction.hh>

namespace assetmanager
{

NewAssetAction::NewAssetAction(const QString &name, QObject *parent)
  : QAction(name, parent)
{
}


void NewAssetAction::SetLocator(const vkResourceLocator &locator)
{
  m_locator = locator;
}

const vkResourceLocator &NewAssetAction::GetLocator() const
{
  return m_locator;
}


bool NewAssetAction::activate()
{
  return false;
}

}

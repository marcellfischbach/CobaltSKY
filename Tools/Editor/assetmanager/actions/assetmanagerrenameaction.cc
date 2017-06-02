
#include <assetmanager/actions/assetmanagerrenameaction.hh>
#include <assetmanager/actions/assetmanagerrenamedialog.hh>
#include <assetmanager/assetmanagercontentmodelentry.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagerrenamer.hh>
#include <valkyrie/core/vkfileinfo.hh>
#include <QString>
#include <QList>

AssetManagerRenameAction::AssetManagerRenameAction()
  : AssetManagerAction()
{

}

AssetManagerRenameAction::~AssetManagerRenameAction()
{

}

bool AssetManagerRenameAction::ShouldShow(AssetManagerWidget *assetManager) const
{
  return true;
}

bool AssetManagerRenameAction::IsEnabled(AssetManagerWidget *assetManager) const
{
  const QList<const AssetManagerContentModelEntry*> entries = assetManager->GetSelectedAssets();
  return entries.size();
}

QString AssetManagerRenameAction::GetMenuEntryName(AssetManagerWidget *assetManager) const
{
  const QList<const AssetManagerContentModelEntry*> entries = assetManager->GetSelectedAssets();
  if (entries.isEmpty())
  {
    return QObject::tr("Rename");
  }
  const AssetManagerContentModelEntry *entry = entries.first();
  return QObject::tr("Rename %1").arg(entry->GetEntryName());
}

bool AssetManagerRenameAction::PerformAction(AssetManagerWidget *assetManager) const
{
  const QList<const AssetManagerContentModelEntry*> entries = assetManager->GetSelectedAssets();
  if (entries.isEmpty())
  {
    return false;
  }
  const AssetManagerContentModelEntry *entry = entries.first();

  AssetManagerRenameDialog dlg(assetManager);
  dlg.SetName(entry->GetEntryName());
  if (dlg.exec())
  {
    vkResourceLocator from = entry->GetLocator();
    vkFileInfo info(from.GetResourceFile());
    std::string newName = info.GetLocation() + std::string("/") + std::string((const char*)dlg.GetName().toLatin1()) + "." + info.GetExtension();
    vkResourceLocator to(newName, from.GetResourceName(), from.GetResourceEntry());

    if (from == to)
    {
      return false;
    }

    AssetManagerRenamer renamer(from, to);
    return renamer.Execute();
  }

  return true;
}

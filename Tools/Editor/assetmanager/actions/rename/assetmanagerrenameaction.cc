
#include <assetmanager/actions/rename/assetmanagerrenameaction.hh>
#include <assetmanager/actions/rename/assetmanagerrenamedialog.hh>
#include <assetmanager/assetmanagercontentmodelentry.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagerrenamer.hh>
#include <cobalt/core/csfileinfo.hh>
#include <editor.hh>
#include <project/project.hh>
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
//  const QList<const AssetManagerContentModelEntry*> entries = assetManager->GetSelectedAssets();
  return false;// entries.size();
}

QString AssetManagerRenameAction::GetMenuEntryName(AssetManagerWidget *assetManager) const
{
  /*
  const QList<const AssetManagerContentModelEntry*> entries = assetManager->GetSelectedAssets();
  if (entries.isEmpty())
  {
    return QObject::tr("Rename");
  }
  const AssetManagerContentModelEntry *entry = entries.first();
  return QObject::tr("Rename %1").arg(entry->GetEntryName());
  */
  return QObject::tr("Rename");
}

bool AssetManagerRenameAction::PerformAction(AssetManagerWidget *assetManager) const
{
  /*
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
    csResourceLocator from = entry->GetLocator();
    csFileInfo info(from.GetResourceFile());
    std::string newName = info.GetLocation() + std::string("/") + std::string((const char*)dlg.GetName().toLatin1()) + "." + info.GetExtension();
    csResourceLocator to(newName, from.GetResourceName(), from.GetResourceEntry());

    if (from == to)
    {
      return false;
    }

    return Editor::Get()->GetProject()->Rename(from, to);
  }
  */
  return false;
}

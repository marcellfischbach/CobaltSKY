
#include <editor/assetmanager/actions/delete/assetmanagerdeleteaction.hh>
#include <editor/assetmanager/actions/delete/assetmanagerdeletedialog.hh>
#include <editor/assetmanager/contextmenu/contextmenucategory.hh>
#include <editor/assetmanager/contextmenu/contextmenuentry.hh>
#include <editor/assetmanager/assetmanagerwidget.hh>
#include <editor/assetmodel/asset.hh>
#include <editor/assetmodel/model.hh>
#include <editor/assetmodel/modeltransaction.hh>
#include <editor/assetmodel/vfsentry.hh>
#include <editor/editor.hh>
#include <editor/mainwindow.hh>
#include <cobalt/core/csvfs.hh>
#include <vector>
#include <QObject>

namespace asset::actions
{


  void AssetManagerDeleteContextMenu::Create(asset::model::Entry *entry, asset::contextmenu::Builder builder)
  {
    if (entry->IsRoot() || entry->IsVFSEntry())
    {
      return;
    }

    std::string name = "Delete '" + entry->GetDisplayName() + "'";
    builder.AddAction(asset::contextmenu::Category::Edit(), name)
      ->SetAction(new AssetManagerDeleteAction(entry));
  }



  AssetManagerDeleteAction::AssetManagerDeleteAction(asset::model::Entry *entry)
    : asset::actions::ActionCallback()
    , m_entry(entry)
  {

  }

  AssetManagerDeleteAction::~AssetManagerDeleteAction()
  {

  }

  void AssetManagerDeleteAction::Callback()
  {
    AssetManagerDeleteDialog dlg(Editor::Get()->GetMainWindow());
    dlg.SetAssetName(m_entry->GetDisplayName());
    int res = dlg.exec();

    if (res != QDialog::Accepted)
    {
      return;
    }

    bool deleteOverloaded = dlg.IsDeleteOverloaded();
    bool deleteSuper = dlg.IsDeleteSuper();


    int entityPriority = m_entry->GetVFSEntryPriority();

    const std::set<asset::model::Entry*> &otherEntriesCopy = std::set<asset::model::Entry*>(m_entry->GetModel()->GetAllEntriesFor(m_entry->GetResourceLocator()));
    asset::model::ModelTransaction tr;
    try
    {
      for (auto deleteEntry : otherEntriesCopy)
      {
        const asset::model::VFSEntry *renameVFSEntry = deleteEntry->GetVFSEntry();
        int deletePriority = renameVFSEntry->GetPriority();

        if (deletePriority < entityPriority && !deleteSuper)
        {
          continue;
        }
        else if (deletePriority > entityPriority && !deleteOverloaded)
        {
          continue;
        }

        deleteEntry->Delete(tr);

      }
      tr.Commit();
    }
    catch (const std::exception &e)
    {
      tr.Rollback();
    }
  }


}



#include <editor/assetmanager/actions/import/assetmanagerimportaction.hh>
#include <editor/assetmanager/actions/import/assetmanagerimporterdialog.hh>
#include <editor/assetmanager/contextmenu/contextmenucategory.hh>
#include <editor/assetmanager/contextmenu/contextmenuentry.hh>
#include <editor/assetmanager/assetmanagerwidget.hh>
#include <editor/assetmodel/asset.hh>
#include <editor/assetmodel/model.hh>
#include <editor/assetmodel/modeltransaction.hh>
#include <editor/assetmodel/vfsentry.hh>
#include <editor/editor.hh>
#include <editor/importerfactorymanager.hh>
#include <editor/mainwindow.hh>
#include <editor/iassetimporterfactory.hh>
#include <cobalt/core/csvfs.hh>
#include <vector>
#include <QObject>
#include <QDir>
#include <QFileDialog>

namespace asset::actions
{


  void AssetManagerImportContextMenu::Create(asset::model::Entry *entry, asset::contextmenu::Builder builder)
  {
    if (entry->IsRoot())
    {
      return;
    }

    builder.AddAction(asset::contextmenu::Category::Import(), "Import")
      ->SetAction(new AssetManagerImportAction(entry));
  }



  AssetManagerImportAction::AssetManagerImportAction(asset::model::Entry *entry)
    : asset::actions::ActionCallback()
    , m_entry(entry)
  {

  }

  AssetManagerImportAction::~AssetManagerImportAction()
  {

  }

  static std::string get_extension_string(const std::vector<std::string> &extensions)
  {
    std::string extensionString = "";
    for (const std::string &ext : extensions)
    {
      extensionString += " *." + ext;
    }
    if (extensionString.empty())
    {
      return "";
    }
    return extensionString.substr(1);
  }

  static QString  get_filter_string(const std::vector<iAssetImporterFactory*> &factories)
  {
    std::vector<std::string> allFilters;
    std::string filterString = "";
    for (iAssetImporterFactory *factory : factories)
    {
      filterString += ";;" + factory->GetName() + "(" + get_extension_string(factory->GetExtensions()) + ")";
      for (const std::string &ext : factory->GetExtensions())
      {
        allFilters.push_back(ext);
      }
    }

    filterString = "All (" + get_extension_string(allFilters) + ")" + filterString;
    return QString(filterString.c_str());
  }

  void AssetManagerImportAction::Callback()
  {
    std::vector<iAssetImporterFactory*> factories = ImporterFactoryManager::Get()->GetFactories();
    static QString lastPath = QDir::homePath();
    QStringList result = QFileDialog::getOpenFileNames(
      Editor::Get()->GetMainWindow(),
      QObject::tr("Select files for import..."),
      lastPath,
      get_filter_string(factories)
    );

    if (result.isEmpty())
    {
      return;
    }

    // save the selected path... will show the import dialog in the same directory the next time
    lastPath = QFileInfo(result[0]).absolutePath();


    AssetManagerImporterDialog *dlg = new AssetManagerImporterDialog(Editor::Get()->GetAssetManager(), m_entry->AsFolder());
    for (const QString &path : result)
    {
      for (iAssetImporterFactory *factory : factories)
      {
        std::filesystem::path stdpath(std::string(path.toLatin1().data()));
        if (factory->CanImport(stdpath))
        {
          std::vector<iAssetImporter*> importers = factory->Import(stdpath);
          dlg->AddImporter(importers);
          break;
        }
      }
    }

    dlg->setVisible(true);
    /*
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
    */
  }


}

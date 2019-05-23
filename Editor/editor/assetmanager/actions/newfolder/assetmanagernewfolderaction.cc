
#include <editor/assetmanager/actions/newfolder/assetmanagernewfolderaction.hh>
#include <editor/assetmanager/actions/newfolder/assetmanagernewfolderdialog.hh>
#include <editor/assetmanager/contextmenu/contextmenucategory.hh>
#include <editor/assetmanager/contextmenu/contextmenuentry.hh>
#include <editor/assetmodel/asset.hh>
#include <editor/assetmodel/model.hh>
#include <editor/assetmodel/modeltransaction.hh>
#include <editor/assetmodel/vfsentry.hh>
#include <editor/editor.hh>
#include <editor/mainwindow.hh>
#include <cobalt/core/csvfs.hh>
#include <vector>
#include <QObject>
#include <QMessageBox>
#include <editor/assetmanager/assetmanagerwidget.hh>
#include <filesystem>


namespace asset::actions
{


	void AssetManagerNewFolderContextMenu::Create(asset::model::Entry *entry, asset::contextmenu::Builder builder)
	{
    if (entry->IsRoot())
		{
			return;
		}

		std::string name = "New folder";
		builder.AddAction(asset::contextmenu::Category::New(), name)
			->SetAction(new AssetManagerNewFolderAction(entry));
	}



	AssetManagerNewFolderAction::AssetManagerNewFolderAction(asset::model::Entry *entry)
		: asset::actions::ActionCallback()
		, m_entry(entry)
	{

	}

  AssetManagerNewFolderAction::~AssetManagerNewFolderAction()
	{

	}

	void AssetManagerNewFolderAction::Callback()
	{
    if (!m_entry)
    {
      QMessageBox::warning(Editor::Get()->GetMainWindow(),
        Editor::Get()->GetApplicationTitle().c_str(),
        QObject::tr("No valid entry selected."),
        QMessageBox::Close);
      return;
    }

    asset::model::Folder *folder = m_entry->FindFolder();
    if (!folder)
    {
      QMessageBox::warning(Editor::Get()->GetMainWindow(),
        Editor::Get()->GetApplicationTitle().c_str(),
        QObject::tr("No valid folder selected."),
        QMessageBox::Close);
      return;
    }
		AssetManagerNewFolderDialog dlg(Editor::Get()->GetMainWindow());
		int res = dlg.exec();

		if (res != QDialog::Accepted)
		{
			return;
		}

	
		std::string newName = dlg.GetFolderName();
    if (newName.empty())
    {
      QMessageBox::warning(Editor::Get()->GetMainWindow(),
        Editor::Get()->GetApplicationTitle().c_str(),
        QObject::tr("Folder name must not be empty."),
        QMessageBox::Close);
      return;

    }
    cs::ResourceLocator newLocator = folder->CreateResourceLocator(newName);
    std::string newPathStr = cs::VFS::Get()->GetAbsolutePath(newLocator, cs::VFS::DontCheckExistence);
    if (newPathStr.empty())
    {
      QMessageBox::warning(Editor::Get()->GetMainWindow(),
        Editor::Get()->GetApplicationTitle().c_str(),
        QObject::tr("Unable to generate absolute path."),
        QMessageBox::Close);
      return;
    }

    std::filesystem::path path(newPathStr);
    if (std::filesystem::exists(path))
    {
      QMessageBox::warning(Editor::Get()->GetMainWindow(),
        Editor::Get()->GetApplicationTitle().c_str(),
        QObject::tr("The given folder already exists."),
        QMessageBox::Close);
      return;
    }

    try
    {
      std::filesystem::create_directories(path);
    }
    catch (const std::exception &e)
    {


    }
    if (!std::filesystem::exists(path))
    {
      QMessageBox::warning(Editor::Get()->GetMainWindow(),
        Editor::Get()->GetApplicationTitle().c_str(),
        QObject::tr("The given folder could not be created."),
        QMessageBox::Close);
      return;
    }

    asset::model::ModelTransaction tr;
    try
    {
      asset::model::Folder *newFolder = m_entry->GetModel()->CreateFolder(newName);
      folder->Add(newFolder, tr);
    }
    catch (const std::exception &e)
    {
      tr.Rollback();
      return;
    }
    tr.Commit();
  }


}


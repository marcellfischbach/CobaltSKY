
#include <assetmanager/actions/newfolder/assetmanagernewfolderaction.hh>
#include <assetmanager/actions/newfolder/assetmanagernewfolderdialog.hh>
#include <assetmanager/contextmenu/contextmenucategory.hh>
#include <assetmanager/contextmenu/contextmenuentry.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/model.hh>
#include <assetmodel/modeltransaction.hh>
#include <assetmodel/vfsentry.hh>
#include <editor.hh>
#include <mainwindow.hh>
#include <cobalt/core/csvfs.hh>
#include <vector>
#include <QObject>
#include <assetmanager/assetmanagerwidget.hh>

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
		AssetManagerNewFolderDialog dlg(Editor::Get()->GetMainWindow());
		int res = dlg.exec();

		if (res != QDialog::Accepted)
		{
			return;
		}

	
		std::string newName = dlg.GetFolderName();

	}


}


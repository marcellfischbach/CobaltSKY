
#include <assetmanager/contextmenu/contextmenubuilder.hh>
#include <assetmanager/contextmenu/contextmenucategory.hh>
#include <assetmanager/contextmenu/contextmenuentry.hh>
#include <QMenu>
#include <iostream>


namespace asset::contextmenu
{

	Builder::Builder(Folder *folder)
		: m_folder(folder)
	{

	}

	Builder Builder::GetFolder(const Category &category, const std::string &name)
	{
		for (Entry *entry : m_folder->GetEntries())
		{
			Folder *folder = entry->AsFolder();
			if (folder && folder->Matches(category, name))
			{
				return Builder(folder);
			}
		}

		Folder *folder = new Folder(category, name);
		m_folder->Add(folder);
		return Builder(folder);
	}

	Action *Builder::AddAction(const Category &category, const std::string &name)
	{
		Action *action = new Action(category, name);
		m_folder->Add(action);
		return action;
	}


	QMenu *Builder::Create()
	{
		QMenu *rootMenu = new QMenu();
		Create(rootMenu, m_folder);
		return rootMenu;
	}

	void Builder::Create(QMenu *menu, Folder *folder)
	{
		if (folder->GetEntries().empty())
		{
			return;
		}
		folder->Sort();

		Category lastCategory = folder->GetEntries()[0]->GetCategory();
		for (Entry *child : folder->GetEntries())
		{
			const Category &category = child->GetCategory();
			if (category != lastCategory)
			{
				menu->addSeparator();
				lastCategory = category;
			}


			if (child->IsAction())
			{
				Action *childAction = child->AsAction();
				QAction *qAction = menu->addAction(QString(childAction->GetName().c_str()));

				if (childAction->GetAction())
				{
					QObject::connect(qAction, &QAction::triggered, [childAction]() {childAction->Callback(); });
				}
				// connect the qAction somehow with the action
			}
			else if (child->IsFolder())
			{
				Folder *childFolder = child->AsFolder();
				QMenu *subMenu = menu->addMenu(QString(childFolder->GetName().c_str()));
				Create(subMenu, childFolder);
			}
		}
	}

}
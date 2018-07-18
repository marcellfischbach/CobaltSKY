
#include <assetmanager/contextmenu/contextmenuhandler.hh>
#include <assetmanager/contextmenu/contextmenubuilder.hh>
#include <assetmanager/contextmenu/contextmenucategory.hh>
#include <assetmanager/contextmenu/contextmenuentry.hh>
#include <QMenu>
#include <iostream>

namespace asset::contextmenu
{


	ContextMenuResult::ContextMenuResult(QMenu *menu, Folder *folder)
		: m_menu(menu)
		, m_folder(folder)
	{
	}

	ContextMenuResult::ContextMenuResult(ContextMenuResult && other)
	{
		m_menu = other.m_menu;
		m_folder = other.m_folder;
		other.m_folder = 0;
		other.m_menu = 0;
	}

	ContextMenuResult::~ContextMenuResult()
	{
		if (m_menu)
		{
			delete m_menu;
			m_menu = 0;
		}
		if (m_folder)
		{
			delete m_folder;
			m_folder = 0;
		}
	}

	QMenu *ContextMenuResult::GetMenu()
	{
		return m_menu;
	}

	QMenu *ContextMenuResult::operator->()
	{
		return m_menu;
	}



	Handler::Handler()
	{

	}



	Handler *Handler::Get()
	{
		static Handler handler;
		return &handler;
	}



	void Handler::AddFactory(Handler::ItemFactory *factory)
	{
		if (std::find(m_factories.begin(), m_factories.end(), factory) != m_factories.end())
		{
			return;
		}

		m_factories.push_back(factory);
	}

	void Handler::RemoveFactory(Handler::ItemFactory *factory)
	{
		auto it = std::find(m_factories.begin(), m_factories.end(), factory);
		if (it != m_factories.end())
		{
			m_factories.erase(it);
		}
	}

	Folder *Handler::CreateRoot() const
	{
		return new Folder(Category::Root(), "");
	}

	ContextMenuResult Handler::BuildContextMenu(asset::model::Entry *entry) const
	{
		std::vector<asset::contextmenu::Entry*> contextEntries;
		Folder *root = new Folder(Category::Root(), "");
		Builder builder(root);
		for (ItemFactory *factory : m_factories)
		{
			if (factory)
			{
				factory->Create(entry, builder);
			}
		}

		return ContextMenuResult(builder.Create(), root);
	}


}
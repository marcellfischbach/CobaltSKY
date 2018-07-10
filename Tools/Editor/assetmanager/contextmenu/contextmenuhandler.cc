
#include <assetmanager/contextmenu/contextmenuhandler.hh>
#include <assetmanager/contextmenu/contextmenubuilder.hh>
#include <assetmanager/contextmenu/contextmenucategory.hh>
#include <assetmanager/contextmenu/contextmenuentry.hh>
#include <QMenu>


namespace asset::contextmenu
{

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

	QMenu *Handler::BuildContextMenu(asset::model::Entry *entry) const
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

		QMenu *menu = builder.Create();
		delete root;
		return menu;
	}


}
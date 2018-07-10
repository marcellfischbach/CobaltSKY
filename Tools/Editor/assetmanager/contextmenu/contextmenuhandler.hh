
#pragma once

#include <vector>
#include <assetmanager/contextmenu/contextmenubuilder.hh>

class QMenu;

namespace asset::model
{
	class Entry;
}

namespace asset::contextmenu
{

	class Entry;
	class Handler
	{
	public:
		struct ItemFactory
		{
			virtual void Create(asset::model::Entry *entry, Builder builder) = 0;
		};

	public:
		static Handler *Get();

		QMenu *BuildContextMenu(asset::model::Entry *entry) const;

		void AddFactory(ItemFactory *factory);
		void RemoveFactory(ItemFactory *factory);

	private:
		Handler();

		std::vector<ItemFactory*> m_factories;
	};


}
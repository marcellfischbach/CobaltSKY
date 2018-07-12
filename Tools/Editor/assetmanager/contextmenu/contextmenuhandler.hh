
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
	class Folder;
	class ContextMenuResult
	{
	public:
		ContextMenuResult(QMenu *menu, Folder *folder);
		ContextMenuResult(ContextMenuResult && other);
		~ContextMenuResult();
		QMenu *GetMenu();
		QMenu *operator->();

	private:
		QMenu * m_menu;
		Folder *m_folder;
	};

	class Handler
	{
	public:
		struct ItemFactory
		{
			virtual ~ItemFactory () { }
			virtual void Create(asset::model::Entry *entry, Builder builder) = 0;
		};

	public:
		static Handler *Get();

		Folder *CreateRoot() const;
		ContextMenuResult BuildContextMenu(asset::model::Entry *entry) const;

		void AddFactory(ItemFactory *factory);
		void RemoveFactory(ItemFactory *factory);

	private:
		Handler();

		std::vector<ItemFactory*> m_factories;
	};


}
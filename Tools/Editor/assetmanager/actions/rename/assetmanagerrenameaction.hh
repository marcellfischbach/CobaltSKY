
#include <assetmanager/actions/assetmanageractioncallback.hh>
#include <assetmanager/contextmenu/contextmenuhandler.hh>

namespace asset::model
{
	class Entry;
}

namespace asset::actions
{

	class AssetManagerRenameContextMenu : public  asset::contextmenu::Handler::ItemFactory
	{
	public:
		virtual ~AssetManagerRenameContextMenu () { }
		virtual void Create(asset::model::Entry *entry, asset::contextmenu::Builder builder);
	};

	class AssetManagerRenameAction : public asset::actions::ActionCallback
	{
	public:
		AssetManagerRenameAction(asset::model::Entry *entry);
		virtual ~AssetManagerRenameAction();


		virtual void Callback();

	private:
		asset::model::Entry *m_entry;
	};
}

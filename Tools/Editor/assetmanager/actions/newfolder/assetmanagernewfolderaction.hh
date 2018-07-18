
#include <assetmanager/actions/assetmanageractioncallback.hh>
#include <assetmanager/contextmenu/contextmenuhandler.hh>

namespace asset::model
{
	class Entry;
}

namespace asset::actions
{

	class AssetManagerNewFolderContextMenu : public  asset::contextmenu::Handler::ItemFactory
	{
	public:
		virtual ~AssetManagerNewFolderContextMenu() { }
		virtual void Create(asset::model::Entry *entry, asset::contextmenu::Builder builder);
	};

	class AssetManagerNewFolderAction : public asset::actions::ActionCallback
	{
	public:
    AssetManagerNewFolderAction(asset::model::Entry *entry);
		virtual ~AssetManagerNewFolderAction();


		virtual void Callback();

	private:
		asset::model::Entry *m_entry;
	};
}

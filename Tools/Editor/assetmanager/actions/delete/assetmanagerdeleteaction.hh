
#include <assetmanager/actions/assetmanageractioncallback.hh>
#include <assetmanager/contextmenu/contextmenuhandler.hh>

namespace asset::model
{
  class Entry;
}

namespace asset::actions
{

  class AssetManagerDeleteContextMenu : public  asset::contextmenu::Handler::ItemFactory
  {
  public:
    virtual ~AssetManagerDeleteContextMenu() { }
    virtual void Create(asset::model::Entry *entry, asset::contextmenu::Builder builder);
  };

  class AssetManagerDeleteAction : public asset::actions::ActionCallback
  {
  public:
    AssetManagerDeleteAction(asset::model::Entry *entry);
    virtual ~AssetManagerDeleteAction();


    virtual void Callback();

  private:
    asset::model::Entry *m_entry;
  };
}

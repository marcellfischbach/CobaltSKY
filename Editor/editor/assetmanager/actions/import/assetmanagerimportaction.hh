
#include <editor/editorexport.hh>
#include <editor/assetmanager/actions/assetmanageractioncallback.hh>
#include <editor/assetmanager/contextmenu/contextmenuhandler.hh>

namespace asset::model
{
  class Entry;
}

namespace asset::actions
{

  class AssetManagerImportContextMenu : public  asset::contextmenu::Handler::ItemFactory
  {
  public:
    virtual ~AssetManagerImportContextMenu() { }
    virtual void Create(asset::model::Entry *entry, asset::contextmenu::Builder builder);
  };

  class AssetManagerImportAction : public asset::actions::ActionCallback
  {
  public:
    AssetManagerImportAction(asset::model::Entry *entry);
    virtual ~AssetManagerImportAction();


    virtual void Callback();

  private:
    asset::model::Entry *m_entry;
  };
}

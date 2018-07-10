

#include <editors/materialeditor/materialeditorcontextmenufactory.hh>
#include <assetmanager/contextmenu/contextmenuentry.hh>


void MaterialEditorContextMenuFactory::Create(asset::model::Entry *entry, asset::contextmenu::Builder builder)
{

	asset::contextmenu::Action* action = builder
		.GetFolder(asset::contextmenu::Category::New(), "New")
		.AddAction(asset::contextmenu::Category::Default(), "Material");
}



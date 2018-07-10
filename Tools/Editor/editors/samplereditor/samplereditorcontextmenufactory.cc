

#include <editors/samplereditor/samplereditorcontextmenufactory.hh>
#include <assetmanager/contextmenu/contextmenuentry.hh>


void SamplerEditorContextMenuFactory::Create(asset::model::Entry *entry, asset::contextmenu::Builder builder)
{

	asset::contextmenu::Action* action = builder
		.GetFolder(asset::contextmenu::Category::New(), "New")
		.AddAction(asset::contextmenu::Category::Default(), "Sampler");
}



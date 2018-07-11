
#pragma once

#include <assetmanager/contextmenu/contextmenuhandler.hh>

class SamplerEditorContextMenuFactory : public asset::contextmenu::Handler::ItemFactory
{

public:
	void Create(asset::model::Entry *entry, asset::contextmenu::Builder contextEntries);

};

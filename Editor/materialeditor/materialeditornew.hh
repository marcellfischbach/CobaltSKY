
#pragma once

#include <materialeditor/materialeditorexport.hh>
#include <editor/assetmanager/contextmenu/contextmenuhandler.hh>
#include <editor/assetmanager/actions/new/assetmanagernewassetaction.hh>

class MATERIALEDITOR_API MaterialEditorContextMenuFactory : public asset::contextmenu::Handler::ItemFactory
{

public:
  void Create(asset::model::Entry *entry, asset::contextmenu::Builder contextEntries);

};


class cs::file::File;
class MATERIALEDITOR_API MaterialCreator : public asset::actions::NewAssetCreator
{
public:
  MaterialCreator(const std::string &name, const std::string &defaultName);

  bool Create(cs::file::File &file);
};
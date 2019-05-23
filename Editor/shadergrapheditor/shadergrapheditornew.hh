
#pragma once

#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <editor/assetmanager/contextmenu/contextmenuhandler.hh>
#include <editor/assetmanager/actions/new/assetmanagernewassetaction.hh>

class ShaderGraphEditorContextMenuFactory : public asset::contextmenu::Handler::ItemFactory
{

public:
  void Create(asset::model::Entry *entry, asset::contextmenu::Builder contextEntries);

};


class cs::file::File;
class ShaderGraphCreator : public asset::actions::NewAssetCreator
{
public:
  ShaderGraphCreator(const std::string &name, const std::string &defaultName);

  bool Create(cs::file::File &file);
};
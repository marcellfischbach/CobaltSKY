#pragma once

#include <AssetManager/NewAssetAction.hh>

class vkResourceLocator;

namespace shadergraph
{

/*
class CreateNewShaderGraph
{
public:
  void CreateNew(const vkResourceLocator &locator);

private:
  CreateNewShaderGraph();
};
*/

class NewShaderGraphAction : public assetmanager::NewAssetAction
{
public:
  NewShaderGraphAction (QObject *parent);

  bool activate ();

};


}

#pragma once

#include <AssetManager/NewAssetAction.hh>

class vkResourceLocator;

namespace shadergraph
{


class NewShaderGraphAction : public assetmanager::NewAssetAction
{
public:
  NewShaderGraphAction (QObject *parent);

  bool activate ();

};


}

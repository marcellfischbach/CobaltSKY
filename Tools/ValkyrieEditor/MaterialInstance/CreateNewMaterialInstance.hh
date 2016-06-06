#pragma once

#include <AssetManager/NewAssetAction.hh>

class vkResourceLocator;

namespace materialinstance
{


class NewMaterialInstanceAction : public assetmanager::NewAssetAction
{
public:
  NewMaterialInstanceAction (QObject *parent);

  bool activate ();

};


}

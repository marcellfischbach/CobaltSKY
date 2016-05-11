

#include <AssetManager/Utils.hh>
#include <Valkyrie/Core/ResourceManager.hh>

namespace assetmanager
{


QString GetNameFromResource(const vkResourceLocator &locator)
{
  QString name(locator.GetResourceFile().c_str());
  int idx = name.lastIndexOf("/");
  if (idx != -1)
  {
    name = name.right(name.length() - idx - 1);
  }
  if (name.endsWith(".asset"))
  {
    name = name.left(name.length() - 6);
  }
  return name;
}

}


#pragma once

namespace cs
{
class ResourceManager;

/**
* \ingroup loading
*/
class Loaders
{
public:
  static void Register(cs::ResourceManager *resourceManager);

private:
  Loaders();
};

}

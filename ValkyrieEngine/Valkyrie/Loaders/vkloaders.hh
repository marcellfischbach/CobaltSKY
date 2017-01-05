
#pragma once

class vkResourceManager;

/**
* \ingroup loading
*/
class vkLoaders
{
public:
  static void Register(vkResourceManager *resourceManager);

private:
  vkLoaders();
};
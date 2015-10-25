
#pragma once

class vkResourceManager;

class vkLoaders
{
public:
  static void Register(vkResourceManager *resourceManager);

private:
  vkLoaders();
};
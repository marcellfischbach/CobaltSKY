
#pragma once

class csResourceManager;

/**
* \ingroup loading
*/
class csLoaders
{
public:
  static void Register(csResourceManager *resourceManager);

private:
  csLoaders();
};
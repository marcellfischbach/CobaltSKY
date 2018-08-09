#pragma once

#include <qstring.h>

class vkResourceLocator;

namespace assetmanager
{




extern QString GetNameFromResource(const vkResourceLocator &locator);
extern QString GetTypeOfResource(const vkResourceLocator &locator);


}


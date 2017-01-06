#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <map>

class vkEntityXMLLoader;
class vkEntityStateXMLLoader;
/**
* \ingroup loading
*/
class VKE_API vkEntityLoaderRegistry
{
public:
  static vkEntityLoaderRegistry *Get();

  void RegisterLoader(const vkEntityXMLLoader *entityLoader);
  void RegisterLoader(const vkEntityStateXMLLoader *entityLoader);
  const vkEntityXMLLoader *GetEntityLoader(const vkClass *clazz) const;
  const vkEntityStateXMLLoader *GetEntityStateLoader(const vkClass *clazz) const;

private:
  vkEntityLoaderRegistry();

  std::map<const vkClass*, const vkEntityXMLLoader*> m_entityLoaders;
  std::map<const vkClass*, const vkEntityStateXMLLoader*> m_entityStateLoaders;
};


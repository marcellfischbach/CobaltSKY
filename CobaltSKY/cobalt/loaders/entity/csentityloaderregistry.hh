#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <map>

class csEntityXMLLoader;
class csEntityStateXMLLoader;
/**
* \ingroup loading
*/
class CSE_API csEntityLoaderRegistry
{
public:
  static csEntityLoaderRegistry *Get();

  void RegisterLoader(const csEntityXMLLoader *entityLoader);
  void RegisterLoader(const csEntityStateXMLLoader *entityLoader);
  const csEntityXMLLoader *GetEntityLoader(const csClass *clazz) const;
  const csEntityStateXMLLoader *GetEntityStateLoader(const csClass *clazz) const;

private:
  csEntityLoaderRegistry();

  std::map<const csClass*, const csEntityXMLLoader*> m_entityLoaders;
  std::map<const csClass*, const csEntityStateXMLLoader*> m_entityStateLoaders;
};


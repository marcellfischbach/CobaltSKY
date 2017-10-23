#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <map>

class csEntityXMLLoader;
class csEntityStateXMLLoader;
class csEntityCSFLoader;
class csEntityStateCSFLoader;
/**
* \ingroup loading
*/
class CSE_API csEntityLoaderRegistry
{
public:
  static csEntityLoaderRegistry *Get();

  void RegisterLoader(const csEntityXMLLoader *entityLoader);
  void RegisterLoader(const csEntityStateXMLLoader *entityLoader);
  const csEntityXMLLoader *GetEntityXMLLoader(const csClass *clazz) const;
  const csEntityStateXMLLoader *GetEntityStateXMLLoader(const csClass *clazz) const;

  void RegisterLoader(const csEntityCSFLoader *entityLoader);
  void RegisterLoader(const csEntityStateCSFLoader *entityLoader);
  const csEntityCSFLoader *GetEntityCSFLoader(const csClass *clazz) const;
  const csEntityStateCSFLoader *GetEntityStateCSFLoader(const csClass *clazz) const;


private:
  csEntityLoaderRegistry();

  std::map<const csClass*, const csEntityXMLLoader*> m_entityXMLLoaders;
  std::map<const csClass*, const csEntityStateXMLLoader*> m_entityStateXMLLoaders;

  std::map<const csClass*, const csEntityCSFLoader*> m_entityCSFLoaders;
  std::map<const csClass*, const csEntityStateCSFLoader*> m_entityStateCSFLoaders;
};


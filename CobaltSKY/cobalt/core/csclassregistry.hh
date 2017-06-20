#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <map>

class CSE_API csClassRegistry
{
public:
  static csClassRegistry *Get();

  void Register(const csClass *clazz);
  void Unregister(const csClass *clazz);

  const std::vector<const csClass*> &GetAllClasses() const;

  const csClass *GetClass(const csString &className) const;
  void *CreateClass(const csString &className) const;

  template<typename T>
  T* CreateClass(const csString &className) const
  {
    return reinterpret_cast<T*>(CreateClass(className));
  }

private:
  csClassRegistry();

  std::vector<const csClass*> m_allClasses;
  std::map<csString, const csClass *> m_classes;
};



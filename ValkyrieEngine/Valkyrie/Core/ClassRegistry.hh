#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <map>

class VKE_API vkClassRegistry
{
public:
  static vkClassRegistry *Get();

  void RegisterClass(const vkClass *clazz);

  const std::vector<const vkClass*> &GetAllClasses() const;

  const vkClass *GetClass(const vkString &className) const;
  void *CreateClass(const vkString &className) const;

  template<typename T>
  T* CreateClass(const vkString &className) const
  {
    return reinterpret_cast<T*>(CreateClass(className));
  }

private:
  vkClassRegistry();

  std::vector<const vkClass*> m_allClasses;
  std::map<vkString, const vkClass *> m_classes;
};



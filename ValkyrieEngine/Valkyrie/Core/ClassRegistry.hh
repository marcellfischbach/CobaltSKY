#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <map>

class VKE_API vkClassRegistry
{
public:
  static vkClassRegistry *Get();

  void RegisterClass(const vkClass *clazz);

  const vkClass *GetClass(const vkString &className) const;
  void *CreateClass(const vkString &className) const;

  template<typename T>
  T* CreateClass(const vkString &className) const
  {
    return reinterpret_cast<T*>(CreateClass(className));
  }

private:
  vkClassRegistry();

  std::map<vkString, const vkClass *> m_classes;
};



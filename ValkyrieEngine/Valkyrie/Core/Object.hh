
#ifndef __VALKYRIE_CORE_OBJECT_H__
#define __VALKYRIE_CORE_OBJECT_H__

#define VK_CLASS(...)
#define VK_INTERFACE(...)
#define VK_SUPER(Cls) Cls
#define VK_PROPERTY(...) public: 
#define VK_CLASS_GEN public: \
    virtual const vkClass *GetClass () const;\
    static const vkClass *GetStaticClass (); \
    virtual void *QueryClass(const vkClass *clazz); \
    virtual const void *QueryClass(const vkClass *clazz) const

#define VK_CLASS_GEN_OBJECT \
    VK_CLASS_GEN; \
    virtual void AddRef() \
    { \
      m_refCount++; \
    } \
    virtual void Release() \
    { \
      --m_refCount; \
      if (m_refCount <= 0) \
      { \
          delete this;\
      } \
    } \
    private: \
      int m_refCount
    
#define VK_CLASS_GEN_CONSTR m_refCount = 1

#define VK_WEAK_OBJECT(Super) \
  public: void AddRef () { Super::AddRef (); }\
          void Release () { Super::Release (); }


#define VK_SET(oo, no) if (no) no->AddRef(); if (oo) oo->Release(); oo = no
#define VK_ADDREF(o) if (o) o->AddRef()
#define VK_RELEASE(o) if (o) o->Release()

#include <string>
#include <vector>
#include <Valkyrie/Export.hh>
#include <Valkyrie/Types.hh>

class vkClass;

struct VKE_API IObject
{
  IObject () { }
  virtual ~IObject() { }
  virtual const vkClass *GetClass() const = 0;

  virtual void AddRef() = 0;

  virtual void Release() = 0;

  virtual void *QueryClass(const vkClass *clazz) = 0;
  virtual const void *QueryClass(const vkClass *clazz) const = 0;
};

template<typename T>
T* vkQueryClass(IObject *object)
{
  if (!object)
  {
    return 0;
  }
  return reinterpret_cast<T*>(object->QueryClass(T::GetStaticClass()));
}

template<typename T>
const T* vkQueryClass(const IObject *object)
{
  if (!object)
  {
    return 0;
  }
  return reinterpret_cast<const T*>(object->QueryClass(T::GetStaticClass()));
}


class VKE_API vkProperty
{
public:
  const std::string &GetName() const;
  const std::string &GetTypeName() const;

  template<typename T>
  void Set(IObject *object, T& t) const
  {
    void *value = reinterpret_cast<void*>(&t);
    SetValue(object, value);
  }

  template<typename T>
  const T &Get(IObject *object) const
  {
    const void *data = GetValue(object);
    return *reinterpret_cast<const T*>(data);
  }

protected:
  vkProperty(const std::string &typeName, const std::string &name);

  virtual void SetValue(IObject *object, void *data) const = 0;
  virtual const void *GetValue(const IObject *object) const = 0;


private:
  std::string m_typeName;
  std::string m_name;

};


class VKE_API vkClass
{
public:
  virtual IObject *CreateInstance() const = 0;
  template<typename T>
  T *CreateInstance() const
  {
    IObject *obj = CreateInstance();
    if (!obj)
    {
      return 0;
    }

    T* t = vkQueryClass<T>(obj);
    if (!t)
    {
      obj->Release();
      obj = 0;
    }
    return t;
  }

  size_t GetNumberOfProperties() const;
  const vkProperty *GetProperty(size_t idx) const;
  const vkProperty *GetProperty(const std::string &propName) const;

  size_t GetNumberOfSuperClasses() const;
  const vkClass *GetSuperClass(size_t idx) const;

  const std::string &GetName() const;

  template<typename T>
  bool IsInstanceOf() const
  {
    return IsInstanceOf(T::GetStaticClass());
  }
  bool IsInstanceOf(const vkClass *clazz) const;

protected:
  vkClass(const std::string &name);

  void AddSuperClass(vkClass *parentClass);
  void AddProperty(vkProperty *prop);

private:
  std::string m_name;
  std::vector<vkClass*> m_superClasses;
  std::vector<vkProperty*> m_properties;

};

template<typename T>
T* vkNewClassInstance(const vkClass *clazz)
{
  return reinterpret_cast<T*>(clazz->CreateInstance());
}

#include <Valkyrie/Core/Object.refl.hh>

VK_CLASS()
class VKE_API vkObject : public IObject
{
  VK_CLASS_GEN;

public:
  vkObject();
  virtual ~vkObject();

  virtual void AddRef();
  virtual void Release();

private:
  int m_refCount;


};


#endif /* ! __VALKYRIE_CORE_OBJECT_H__ */



#ifndef __COBALTSKY_CORE_OBJECT_H__
#define __COBALTSKY_CORE_OBJECT_H__

#define CS_CLASS(...)
#define CS_INTERFACE(...)
#define CS_SUPER(Cls) Cls
#define CS_PROPERTY(...) public: 
#define CS_CLASS_GEN public: \
    virtual const csClass *GetClass () const;\
    static const csClass *GetStaticClass (); \
    virtual void *QueryClass(const csClass *clazz); \
    virtual const void *QueryClass(const csClass *clazz) const

#define CS_CLASS_GEN_OBJECT \
    CS_CLASS_GEN; \
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
    
#define CS_CLASS_GEN_CONSTR m_refCount = 1

#define CS_WEAK_OBJECT(Super) \
  public: void AddRef () { Super::AddRef (); }\
          void Release () { Super::Release (); }


#define CS_SET(oo, no) if (no) no->AddRef(); if (oo) oo->Release(); oo = no
#define CS_ADDREF(o) if (o) o->AddRef()
#define CS_RELEASE(o) if (o) o->Release()

#include <string>
#include <vector>
#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>

class csClass;

struct CSE_API iObject
{
  iObject () { }
  virtual ~iObject() { }
  virtual const csClass *GetClass() const = 0;

  virtual void AddRef() = 0;

  virtual void Release() = 0;

  virtual void *QueryClass(const csClass *clazz) = 0;
  virtual const void *QueryClass(const csClass *clazz) const = 0;
};

template<typename T>
T* csQueryClass(iObject *object)
{
  if (!object)
  {
    return 0;
  }
  return reinterpret_cast<T*>(object->QueryClass(T::GetStaticClass()));
}

template<typename T>
const T* csQueryClass(const iObject *object)
{
  if (!object)
  {
    return 0;
  }
  return reinterpret_cast<const T*>(object->QueryClass(T::GetStaticClass()));
}


class CSE_API csProperty
{
public:
  const std::string &GetName() const;
  const std::string &GetTypeName() const;

  template<typename T>
  void Set(iObject *object, T& t) const
  {
    void *value = reinterpret_cast<void*>(&t);
    SetValue(object, value);
  }

  template<typename T>
  const T &Get(iObject *object) const
  {
    const void *data = GetValue(object);
    return *reinterpret_cast<const T*>(data);
  }

protected:
  csProperty(const std::string &typeName, const std::string &name);

  virtual void SetValue(iObject *object, void *data) const = 0;
  virtual const void *GetValue(const iObject *object) const = 0;


private:
  std::string m_typeName;
  std::string m_name;

};


class CSE_API csClass
{
public:
  virtual iObject *CreateInstance() const = 0;
  template<typename T>
  T *CreateInstance() const
  {
    iObject *obj = CreateInstance();
    if (!obj)
    {
      return 0;
    }

    T* t = csQueryClass<T>(obj);
    if (!t)
    {
      obj->Release();
      obj = 0;
    }
    return t;
  }

  size_t GetNumberOfProperties() const;
  const csProperty *GetProperty(size_t idx) const;
  const csProperty *GetProperty(const std::string &propName) const;

  size_t GetNumberOfSuperClasses() const;
  const csClass *GetSuperClass(size_t idx) const;

  const std::string &GetName() const;

  template<typename T>
  bool IsInstanceOf() const
  {
    return IsInstanceOf(T::GetStaticClass());
  }
  bool IsInstanceOf(const csClass *clazz) const;

protected:
  csClass(const std::string &name);

  void AddSuperClass(csClass *parentClass);
  void AddProperty(csProperty *prop);

private:
  std::string m_name;
  std::vector<csClass*> m_superClasses;
  std::vector<csProperty*> m_properties;

};

template<typename T>
T* csNewClassInstance(const csClass *clazz)
{
  return reinterpret_cast<T*>(clazz->CreateInstance());
}

#include <cobalt/core/csclass.refl.hh>

CS_CLASS()
class CSE_API csObject : public iObject
{
  CS_CLASS_GEN;

public:
  csObject();
  virtual ~csObject();

  virtual void AddRef();
  virtual void Release();

private:
  int m_refCount;


};


#endif /* ! __COBALTSKY_CORE_OBJECT_H__ */


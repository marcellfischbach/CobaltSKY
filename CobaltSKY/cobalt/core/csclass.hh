
#ifndef __COBALTSKY_CORE_OBJECT_H__
#define __COBALTSKY_CORE_OBJECT_H__

#define CS_CLASS(...)
#define CS_INTERFACE(...)
#define CS_SUPER(Cls) Cls
#define CS_PROPERTY(...) 
#define CS_FUNCTION(...) 
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

#include <map>
#include <set>
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


enum csValueMemoryMode
{
  eVMM_Value,
  eVMM_Reference,
  eVMM_Pointer,
  eVMM_PointerToPointer,
};

class CSE_API csProperty
{
public:
  const std::string &GetName() const;
  const std::string &GetTypeName() const;

  template<typename T>
  void Set(iObject *object, T& t) const
  {
    const void *const_value = reinterpret_cast<const void*>(&t);
    SetValue(object, const_cast<void*>(const_value));
  }


  template<typename T>
  T Get(iObject *object) const
  {
    const void *data = GetValue(object);
    return *reinterpret_cast<const T*>(data);
  }

  std::set<std::string> GetProperties () const;
  bool HasProperty (const std::string &property) const;
  std::string GetProperty (const std::string &property) const;


protected:
  csProperty(const std::string &typeName, const std::string &name);

  void SetProperty (const std::string &key, const std::string &value = "");
  virtual void SetValue(iObject *object, void *data) const = 0;
  virtual const void *GetValue(const iObject *object) const = 0;


private:
  std::string m_typeName;
  std::string m_name;
  std::map<std::string, std::string> m_properties;

};

class csValueDeclaration
{
public:
  csValueDeclaration(bool isConst = false, const std::string &type = "void", csValueMemoryMode mode = eVMM_Value);

  bool IsConst() const;
  const std::string &GetType() const;
  csValueMemoryMode GetMode() const;

  bool operator==(const csValueDeclaration &other) const;
private:
  bool m_const;
  std::string m_type;
  csValueMemoryMode m_mode;
};

class CSE_API csFunctionAttribute
{
public:
  csFunctionAttribute(const csValueDeclaration &type = csValueDeclaration(), const std::string &name = "");

  const csValueDeclaration &GetType() const;
  const std::string &GetName() const;

private:
  csValueDeclaration m_type;
  std::string m_name;
};

class CSE_API csFunction
{
public:
  const std::string &GetName() const;
  const csValueDeclaration &GetReturnType() const;
  const bool IsVirtual() const;
  const bool IsConst() const;

  size_t GetNumberOfAttributes() const;
  const csFunctionAttribute &GetAttribute(size_t idx) const;

  virtual void Invoke(iObject *obj, ...) const = 0;
  virtual void Invoke(const iObject *obj, ...) const = 0;
  
  //
  // InvokeVoid
  void InvokeVoid(iObject *obj) const { Invoke(obj); }
  template<typename A0>
  void InvokeVoid(iObject *obj, A0 a0) const { Invoke(obj, &a0); }
  template<typename A0, typename A1>
  void InvokeVoid(iObject *obj, A0 a0, A1 a1) const { Invoke(obj, &a0, &a1); }
  template<typename A0, typename A1, typename A2>
  void InvokeVoid(iObject *obj, A0 a0, A1 a1, A2 a2) const { Invoke(obj, &a0, &a1, &a2); }
  template<typename A0, typename A1, typename A2, typename A3>
  void InvokeVoid(iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3) const { Invoke(obj, &a0, &a1, &a2, &a3); }
  template<typename A0, typename A1, typename A2, typename A3, typename A4>
  void InvokeVoid(iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const { Invoke(obj, &a0, &a1, &a2, &a3, &a4); }
  template<typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
  void InvokeVoid(iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const { Invoke(obj, &a0, &a1, &a2, &a3, &a4, &a5); }
  //
  // InvokeVoid Const
  void InvokeVoid(const iObject *obj) const { Invoke(obj); }
  template<typename A0>
  void InvokeVoid(const iObject *obj, A0 a0) const { Invoke(obj, &a0); }
  template<typename A0, typename A1>
  void InvokeVoid(const iObject *obj, A0 a0, A1 a1) const { Invoke(obj, &a0, &a1); }
  template<typename A0, typename A1, typename A2>
  void InvokeVoid(const iObject *obj, A0 a0, A1 a1, A2 a2) const { Invoke(obj, &a0, &a1, &a2); }
  template<typename A0, typename A1, typename A2, typename A3>
  void InvokeVoid(const iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3) const { Invoke(obj, &a0, &a1, &a2, &a3); }
  template<typename A0, typename A1, typename A2, typename A3, typename A4>
  void InvokeVoid(const iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const { Invoke(obj, &a0, &a1, &a2, &a3, &a4); }
  template<typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
  void InvokeVoid(const iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const { Invoke(obj, &a0, &a1, &a2, &a3, &a4, &a5); }
  //
  // InvokeValue
  template<typename R>
  R InvokeValue(iObject *obj) const { R r;  Invoke(obj, &r); return r; }
  template<typename R, typename A0>
  R InvokeValue(iObject *obj, A0 a0) const { R r;  Invoke(obj, &a0, &r); return r; }
  template<typename R, typename A0, typename A1>
  R InvokeValue(iObject *obj, A0 a0, A1 a1) const { R r;  Invoke(obj, &a0, &a1, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2>
  R InvokeValue(iObject *obj, A0 a0, A1 a1, A2 a2) const { R r;  Invoke(obj, &a0, &a1, &a2, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3>
  R InvokeValue(iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3) const { R r;  Invoke(obj, &a0, &a1, &a2, &a3, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
  R InvokeValue(iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const { R r;  Invoke(obj, &a0, &a1, &a2, &a3, &a4, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
  R InvokeValue(iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const { R r;  Invoke(obj, &a0, &a1, &a2, &a3, &a4, &a5, &r); return r; }
  //
  // InvokeValue const
  template<typename R>
  R InvokeValue(const iObject *obj) const { R r;  Invoke(obj, &r); return r; }
  template<typename R, typename A0>
  R InvokeValue(const iObject *obj, A0 a0) const { R r;  Invoke(obj, &a0, &r); return r; }
  template<typename R, typename A0, typename A1>
  R InvokeValue(const iObject *obj, A0 a0, A1 a1) const { R r;  Invoke(obj, &a0, &a1, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2>
  R InvokeValue(const iObject *obj, A0 a0, A1 a1, A2 a2) const { R r;  Invoke(obj, &a0, &a1, &a2, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3>
  R InvokeValue(const iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3) const { R r;  Invoke(obj, &a0, &a1, &a2, &a3, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
  R InvokeValue(const iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const { R r;  Invoke(obj, &a0, &a1, &a2, &a3, &a4, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
  R InvokeValue(const iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const { R r;  Invoke(obj, &a0, &a1, &a2, &a3, &a4, &a5, &r); return r; }
  //
  // InvokeReference
  template<typename R>
  R &InvokeReference(iObject *obj) const { R *r;  Invoke(obj, &r); return *r; }
  template<typename R, typename A0>
  R &InvokeReference(iObject *obj, A0 a0) const { R *r;  Invoke(obj, &a0, &r); return *r; }
  template<typename R, typename A0, typename A1>
  R &InvokeReference(iObject *obj, A0 a0, A1 a1) const { R *r;  Invoke(obj, &a0, &a1, &r); return *r; }
  template<typename R, typename A0, typename A1, typename A2>
  R &InvokeReference(iObject *obj, A0 a0, A1 a1, A2 a2) const { R *r;  Invoke(obj, &a0, &a1, &a2, &r); return *r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3>
  R &InvokeReference(iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3) const { R *r;  Invoke(obj, &a0, &a1, &a2, &a3, &r); return *r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
  R &InvokeReference(iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const { R *r;  Invoke(obj, &a0, &a1, &a2, &a3, &a4, &r); return *r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
  R &InvokeReference(iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const { R *r;  Invoke(obj, &a0, &a1, &a2, &a3, &a4, &a5, &r); return *r; }
  //
  // InvokeReference Const
  template<typename R>
  R &InvokeReference(const iObject *obj) const { R *r;  Invoke(obj, &r); return *r; }
  template<typename R, typename A0>
  R &InvokeReference(const iObject *obj, A0 a0) const { R *r;  Invoke(obj, &a0, &r); return *r; }
  template<typename R, typename A0, typename A1>
  R &InvokeReference(const iObject *obj, A0 a0, A1 a1) const { R *r;  Invoke(obj, &a0, &a1, &r); return *r; }
  template<typename R, typename A0, typename A1, typename A2>
  R &InvokeReference(const iObject *obj, A0 a0, A1 a1, A2 a2) const { R *r;  Invoke(obj, &a0, &a1, &a2, &r); return *r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3>
  R &InvokeReference(const iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3) const { R *r;  Invoke(obj, &a0, &a1, &a2, &a3, &r); return *r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
  R &InvokeReference(const iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const { R *r;  Invoke(obj, &a0, &a1, &a2, &a3, &a4, &r); return *r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
  R &InvokeReference(const iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const { R *r;  Invoke(obj, &a0, &a1, &a2, &a3, &a4, &a5, &r); return *r; }
  //
  // InvokePointer
  template<typename R>
  R *InvokePointer(iObject *obj) const { R *r;  Invoke(obj, &r); return r; }
  template<typename R, typename A0>
  R *InvokePointer(iObject *obj, A0 a0) const { R *r;  Invoke(obj, &a0, &r); return r; }
  template<typename R, typename A0, typename A1>
  R *InvokePointer(iObject *obj, A0 a0, A1 a1) const { R *r;  Invoke(obj, &a0, &a1, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2>
  R *InvokePointer(iObject *obj, A0 a0, A1 a1, A2 a2) const { R *r;  Invoke(obj, &a0, &a1, &a2, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3>
  R *InvokePointer(iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3) const { R *r;  Invoke(obj, &a0, &a1, &a2, &a3, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
  R *InvokePointer(iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const { R *r;  Invoke(obj, &a0, &a1, &a2, &a3, &a4, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
  R *InvokePointer(iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const { R *r;  Invoke(obj, &a0, &a1, &a2, &a3, &a4, &a5, &r); return r; }
  //
  // InvokePointer Const
  template<typename R>
  R *InvokePointer(const iObject *obj) const { R *r;  Invoke(obj, &r); return r; }
  template<typename R, typename A0>
  R *InvokePointer(const iObject *obj, A0 a0) const { R *r;  Invoke(obj, &a0, &r); return r; }
  template<typename R, typename A0, typename A1>
  R *InvokePointer(const iObject *obj, A0 a0, A1 a1) const { R *r;  Invoke(obj, &a0, &a1, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2>
  R *InvokePointer(const iObject *obj, A0 a0, A1 a1, A2 a2) const { R *r;  Invoke(obj, &a0, &a1, &a2, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3>
  R *InvokePointer(const iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3) const { R *r;  Invoke(obj, &a0, &a1, &a2, &a3, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
  R *InvokePointer(const iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) const { R *r;  Invoke(obj, &a0, &a1, &a2, &a3, &a4, &r); return r; }
  template<typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
  R *InvokePointer(const iObject *obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const { R *r;  Invoke(obj, &a0, &a1, &a2, &a3, &a4, &a5, &r); return r; }

protected:
  csFunction(bool isVirtual, const csValueDeclaration &returnType, const std::string &name, bool isConst);

  void AddAttribute(const csFunctionAttribute &attribute);

private:
  bool m_virtual;
  bool m_const;
  std::string m_name;
  csValueDeclaration m_returnType;

  std::vector<csFunctionAttribute> m_attributes;
  csFunctionAttribute m_invalid;
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

  size_t GetNumberOfFunctions() const;
  const csFunction *GetFunction(size_t idx) const;
  std::vector<const csFunction*> GetFunction(const std::string &functionName) const;

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
  void AddFunction(csFunction *function);

private:
  std::string m_name;
  std::vector<csClass*> m_superClasses;
  std::vector<csProperty*> m_properties;
  std::vector<csFunction*> m_functions;

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


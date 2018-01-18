
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcelocator.hh>

#include <cobalt/entity/blueprint/csbpproperty.refl.hh>

class CSE_API csBPBaseProperty 
{
public:
  void SetName(const std::string &name);
  const std::string &GetName() const;

  void SetCollection(bool collection);
  bool IsCollection() const;
  void SetCollectionIdx(csUInt64 collectionIdx);
  csUInt64 GetCollectionIdx() const;

  virtual void Apply(iObject *obj);

  virtual void SetValue(iObject *obj) = 0;
  virtual void SetCollectionValue(iObject *obj, csUInt64 idx) = 0;
  virtual void AddCollectionValue(iObject *obj) = 0;

protected:
  csBPBaseProperty();

  const csProperty *GetProperty(const csClass *ownerClass);
  const csFunction *GetSetter(const csClass *ownerClass);
  const csFunction *GetCollectionSetter(const csClass *ownerClass);
  const csFunction *GetCollectionAdder(const csClass *ownerClass);



private:
  const csFunction *GetSetter(const csClass *ownerClass, const std::string &setterName);
  const csFunction *GetCollectionSetter(const csClass *ownerClass, const csProperty *prop, const std::string &setterName, bool absName);
  const csFunction *GetCollectionAdder(const csClass *ownerClass, const csProperty *prop, const std::string &setterName, bool absName);

  std::string m_name;

  bool m_collection;
  csUInt64 m_collectionIdx;

  const csProperty *m_property;

  const csFunction *m_setter;
  const csFunction *m_collectionSetter;
  const csFunction *m_collectionAdder;

};

template<typename T>
class csBPProperty : public csBPBaseProperty
{
public:

  csBPProperty() : csBPBaseProperty() { }
  virtual ~csBPProperty() { }

  virtual void SetValue(iObject *obj)
  {
    const csFunction *setter = GetSetter(obj->GetClass());
    if (setter)
    {
      setter->InvokeVoid(obj, m_value);
    }
  }

  virtual void SetCollectionValue(iObject *obj, csUInt64 idx)
  {
    const csFunction *setter = GetCollectionSetter(obj->GetClass());
    if (setter)
    {
      setter->InvokeVoid(obj, idx, m_value);
    }
  }

  virtual void AddCollectionValue(iObject *obj)
  {
    const csFunction *adder = GetCollectionAdder(obj->GetClass());
    if (adder)
    {
      adder->InvokeVoid(obj, m_value);
    }
  }

  void Set(const T& v)
  {
    m_value = v;
  }

  const T& Get() const
  {
    return m_value;
  }

private:
  T m_value;

};


class CSE_API csBPResourceProperty : public csBPBaseProperty
{
public:

  csBPResourceProperty();
  virtual ~csBPResourceProperty();

  virtual void SetValue(iObject *obj);
  virtual void SetCollectionValue(iObject *obj, csUInt64 idx);
  virtual void AddCollectionValue(iObject *obj);

  const iObject *GetResource() const;

  void SetResourceLocator(const csResourceLocator& locator);
  const csResourceLocator& GetResourceLocator() const;

private:
  iObject *GetResource();;
  csResourceLocator m_locator;

};

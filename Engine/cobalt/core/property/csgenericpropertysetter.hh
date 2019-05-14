#pragma once

#include <cobalt/core/property/cspropertysetter.hh>


template<typename T>
class csGenericPropertySetter : public csPropertySetter
{
public:

  csGenericPropertySetter() : csPropertySetter() { }
  virtual ~csGenericPropertySetter() { }

  virtual void SetValue(cs::iObject *obj)
  {
    const cs::Function *setter = GetSetter(obj->GetClass());
    if (setter)
    {
      setter->InvokeVoid(obj, m_value);
    }
  }

  virtual void SetCollectionValue(cs::iObject *obj, csUInt64 idx)
  {
    const cs::Function *setter = GetCollectionSetter(obj->GetClass());
    if (setter)
    {
      setter->InvokeVoid(obj, idx, m_value);
    }
  }

  virtual void AddCollectionValue(cs::iObject *obj)
  {
    const cs::Function *adder = GetCollectionAdder(obj->GetClass());
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

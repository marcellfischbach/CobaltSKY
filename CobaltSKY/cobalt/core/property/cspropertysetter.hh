
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/property/cspropertysetter.refl.hh>

CS_INTERFACE()
class CSE_API csPropertySetter : public csObject
{
  CS_CLASS_GEN;
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
  csPropertySetter();

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


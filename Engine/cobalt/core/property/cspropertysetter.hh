
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/core/property/cspropertysetter.refl.hh>

CS_CLASS()
class CSE_API csPropertySetter : public cs::Object
{
  CS_CLASS_GEN;
public:
  void SetName(const std::string &name);
  const std::string &GetName() const;

  void SetCollection(bool collection);
  bool IsCollection() const;
  void SetCollectionIdx(csUInt64 collectionIdx);
  csUInt64 GetCollectionIdx() const;

  virtual void Apply(cs::iObject *obj);

  virtual void SetValue(cs::iObject *obj) = 0;
  virtual void SetCollectionValue(cs::iObject *obj, csUInt64 idx) = 0;
  virtual void AddCollectionValue(cs::iObject *obj) = 0;

protected:
  csPropertySetter();

  const cs::Property *GetProperty(const cs::Class *ownerClass);
  const cs::Function *GetSetter(const cs::Class *ownerClass);
  const cs::Function *GetCollectionSetter(const cs::Class *ownerClass);
  const cs::Function *GetCollectionAdder(const cs::Class *ownerClass);



private:
  const cs::Function *GetSetter(const cs::Class *ownerClass, const std::string &setterName);
  const cs::Function *GetSetter(const cs::Class *ownerClass, const std::string &setterName, const std::string &typeName);
  const cs::Function *GetCollectionSetter(const cs::Class *ownerClass, const cs::Property *prop, const std::string &setterName, bool absName);
  const cs::Function *GetCollectionSetter(const cs::Class *ownerClass, const std::string &setterName, const std::string &typeName);
  const cs::Function *GetCollectionAdder(const cs::Class *ownerClass, const cs::Property *prop, const std::string &setterName, bool absName);
  const cs::Function *GetCollectionAdder(const cs::Class *ownerClass, const std::string &setterName, const std::string &typeName);

  std::string m_name;

  bool m_collection;
  csUInt64 m_collectionIdx;

  const cs::Property*m_property;

  const cs::Function *m_setter;
  const cs::Function *m_collectionSetter;
  const cs::Function *m_collectionAdder;

};


CS_CLASS()
class CSE_API csPropertySetterWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(csPropertySetter, csPropertySetterWrapper, csResourceWrapper);
};



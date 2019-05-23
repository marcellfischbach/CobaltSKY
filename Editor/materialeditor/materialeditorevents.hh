
#pragma once
#include <materialeditor/materialeditorexport.hh>

#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <string>

#include <materialeditor/materialeditorevents.refl.hh>

CS_CLASS()
class MATERIALEDITOR_API MaterialEditorAttributeAdded : public cs::Event
{
  CS_CLASS_GEN;
public:
  MaterialEditorAttributeAdded(
    const cs::ResourceLocator &materialLocator,
    const std::string &attributeID,
    const std::string &attributeName
  );


  const cs::ResourceLocator &GetMaterialLocator() const;
  const std::string &GetAttributeID() const;
  const std::string &GetAttributeName() const;
  
private:
  cs::ResourceLocator m_materialLocator;
  std::string m_attributeID;

  std::string m_attributeName;

};


CS_CLASS()
class MATERIALEDITOR_API MaterialEditorAttributeChanged : public cs::Event
{
  CS_CLASS_GEN;
public:
  MaterialEditorAttributeChanged(
    const cs::ResourceLocator &materialLocator,
    const std::string &attributeID,
    const std::string &attributeName
  );


  const cs::ResourceLocator &GetMaterialLocator() const;
  const std::string &GetAttributeID() const;
  const std::string &GetAttributeName() const;

private:
  cs::ResourceLocator m_materialLocator;
  std::string m_attributeID;

  std::string m_attributeName;

};



CS_CLASS()
class MATERIALEDITOR_API MaterialEditorAttributeRemoved : public cs::Event
{
  CS_CLASS_GEN;
public:
  MaterialEditorAttributeRemoved(
    const cs::ResourceLocator &materialLocator,
    const std::string &attributeID
  );


  const cs::ResourceLocator &GetMaterialLocator() const;
  const std::string &GetAttributeID() const;

private:
  csUInt8 pad[7];
  cs::ResourceLocator m_materialLocator;
  std::string m_attributeID;


};



#pragma once
#include <materialeditor/materialeditorexport.hh>

#include <cobalt/core/csevent.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <string>

#include <materialeditor/materialeditorevents.refl.hh>

CS_INTERFACE()
class MATERIALEDITOR_API MaterialEditorAttributeAdded : public csEvent
{
  CS_CLASS_GEN;
public:
  MaterialEditorAttributeAdded(
    const csResourceLocator &materialLocator,
    const std::string &attributeID,
    const std::string &attributeName
  );


  const csResourceLocator &GetMaterialLocator() const;
  const std::string &GetAttributeID() const;
  const std::string &GetAttributeName() const;
  
private:
  csResourceLocator m_materialLocator;
  std::string m_attributeID;

  std::string m_attributeName;

};


CS_INTERFACE()
class MATERIALEDITOR_API MaterialEditorAttributeChanged : public csEvent
{
  CS_CLASS_GEN;
public:
  MaterialEditorAttributeChanged(
    const csResourceLocator &materialLocator,
    const std::string &attributeID,
    const std::string &attributeName
  );


  const csResourceLocator &GetMaterialLocator() const;
  const std::string &GetAttributeID() const;
  const std::string &GetAttributeName() const;

private:
  csResourceLocator m_materialLocator;
  std::string m_attributeID;

  std::string m_attributeName;

};



CS_INTERFACE()
class MATERIALEDITOR_API MaterialEditorAttributeRemoved : public csEvent
{
  CS_CLASS_GEN;
public:
  MaterialEditorAttributeRemoved(
    const csResourceLocator &materialLocator,
    const std::string &attributeID
  );


  const csResourceLocator &GetMaterialLocator() const;
  const std::string &GetAttributeID() const;

private:
  csUInt8 pad[7];
  csResourceLocator m_materialLocator;
  std::string m_attributeID;


};


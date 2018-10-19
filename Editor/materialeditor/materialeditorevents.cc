
#include <materialeditor/materialeditorevents.hh>


MaterialEditorAttributeAdded::MaterialEditorAttributeAdded(const csResourceLocator &materialLocator, const std::string &attributeID, const std::string &attributeName)
  : csEvent ()
  , m_materialLocator(materialLocator)
  , m_attributeID(attributeID)
  , m_attributeName(attributeName)
{
}


const csResourceLocator &MaterialEditorAttributeAdded::GetMaterialLocator() const
{
  return m_materialLocator;
}

const std::string &MaterialEditorAttributeAdded::GetAttributeID() const
{
  return m_attributeID;
}

const std::string &MaterialEditorAttributeAdded::GetAttributeName() const
{
  return m_attributeName;
}






MaterialEditorAttributeChanged::MaterialEditorAttributeChanged(const csResourceLocator &materialLocator, const std::string &attributeID, const std::string &attributeName)
  : csEvent()
  , m_materialLocator(materialLocator)
  , m_attributeID(attributeID)
  , m_attributeName(attributeName)
{
}


const csResourceLocator &MaterialEditorAttributeChanged::GetMaterialLocator() const
{
  return m_materialLocator;
}

const std::string &MaterialEditorAttributeChanged::GetAttributeID() const
{
  return m_attributeID;
}

const std::string &MaterialEditorAttributeChanged::GetAttributeName() const
{
  return m_attributeName;
}





MaterialEditorAttributeRemoved::MaterialEditorAttributeRemoved(const csResourceLocator &materialLocator, const std::string &attributeID)
  : csEvent()
  , m_materialLocator(materialLocator)
  , m_attributeID(attributeID)
{
}


const csResourceLocator &MaterialEditorAttributeRemoved::GetMaterialLocator() const
{
  return m_materialLocator;
}

const std::string &MaterialEditorAttributeRemoved::GetAttributeID() const
{
  return m_attributeID;
}

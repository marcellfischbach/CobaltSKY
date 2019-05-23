
#include <materialeditor/materialeditorevents.hh>


MaterialEditorAttributeAdded::MaterialEditorAttributeAdded(const cs::ResourceLocator &materialLocator, const std::string &attributeID, const std::string &attributeName)
  : cs::Event ()
  , m_materialLocator(materialLocator)
  , m_attributeID(attributeID)
  , m_attributeName(attributeName)
{
}


const cs::ResourceLocator &MaterialEditorAttributeAdded::GetMaterialLocator() const
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






MaterialEditorAttributeChanged::MaterialEditorAttributeChanged(const cs::ResourceLocator &materialLocator, const std::string &attributeID, const std::string &attributeName)
  : cs::Event()
  , m_materialLocator(materialLocator)
  , m_attributeID(attributeID)
  , m_attributeName(attributeName)
{
}


const cs::ResourceLocator &MaterialEditorAttributeChanged::GetMaterialLocator() const
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





MaterialEditorAttributeRemoved::MaterialEditorAttributeRemoved(const cs::ResourceLocator &materialLocator, const std::string &attributeID)
  : cs::Event()
  , m_materialLocator(materialLocator)
  , m_attributeID(attributeID)
{
}


const cs::ResourceLocator &MaterialEditorAttributeRemoved::GetMaterialLocator() const
{
  return m_materialLocator;
}

const std::string &MaterialEditorAttributeRemoved::GetAttributeID() const
{
  return m_attributeID;
}



#include <project/projectassetreference.hh>


ProjectAssetReference::ProjectAssetReference()
{

}

void ProjectAssetReference::SetName(const csString &name)
{
  m_name = name;
}

const csString &ProjectAssetReference::GetName() const
{
  return m_name;
}

void ProjectAssetReference::SetTypeName(const csString &typeName)
{
  m_typeName = typeName;
}

const csString &ProjectAssetReference::GetTypeName() const
{
  return m_typeName;
}


void ProjectAssetReference::SetResourceLocator(const csResourceLocator &resourceLocator)
{
  m_resourceLocator = resourceLocator;
}

const csResourceLocator &ProjectAssetReference::GetResourceLocator() const
{
  return m_resourceLocator;
}



void ProjectAssetReference::AddReference(ProjectAssetReference *reference)
{
  if (!IsReferencing(reference))
  {
    m_references.push_back(reference);
  }
}

void ProjectAssetReference::RemoveReference(ProjectAssetReference *reference)
{
  for (auto it = m_references.begin(); it != m_references.end(); ++it)
  {
    if (*it == reference)
    {
      m_references.erase(it);
      return;
    }
  }
}

void ProjectAssetReference::ClearReference()
{
  m_references.clear();
}

void ProjectAssetReference::AddReferencedBy(ProjectAssetReference *referencedBy)
{
  if (!IsReferencedBy(referencedBy))
  {
    m_referencedBy.push_back(referencedBy);
  }
}

void ProjectAssetReference::RemoveReferencedBy(ProjectAssetReference *referencedBy)
{
  for (auto it = m_referencedBy.begin(); it != m_referencedBy.end(); ++it)
  {
    if (*it == referencedBy)
    {
      m_referencedBy.erase(it);
      return;
    }
  }
}

void ProjectAssetReference::ClearReferencedBy()
{
  m_referencedBy.clear();
}

bool ProjectAssetReference::IsReferencing(ProjectAssetReference *reference) const
{
  for (auto ref : m_references)
  {
    if (ref == reference)
    {
      return true;
    }
  }
  return false;
}

bool ProjectAssetReference::IsReferencedBy(ProjectAssetReference *reference) const
{
  for (auto ref : m_referencedBy)
  {
    if (ref == reference)
    {
      return true;
    }
  }
  return false;
}

const std::vector<ProjectAssetReference*> &ProjectAssetReference::GetReferences() const
{
  return m_references;
}

const std::vector<ProjectAssetReference*> &ProjectAssetReference::GetReferencedBy() const
{
  return m_referencedBy;
}




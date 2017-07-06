

#include <project/projectassetreference.hh>


ProjectAssetReference::ProjectAssetReference()
  : m_parent(0)
  , m_child(0)
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

void ProjectAssetReference::SetPriority(unsigned priority)
{
  m_priority = priority;
}

unsigned ProjectAssetReference::GetPriority() const
{
  return m_priority;
}


void ProjectAssetReference::SetResourceLocator(const csResourceLocator &resourceLocator)
{
  m_resourceLocator = resourceLocator;
}

const csResourceLocator &ProjectAssetReference::GetResourceLocator() const
{
  return m_resourceLocator;
}


void ProjectAssetReference::SetParent(ProjectAssetReference *parent)
{
  m_parent = parent;
}

ProjectAssetReference *ProjectAssetReference::GetParent() const
{
  return m_parent;
}

void ProjectAssetReference::SetChild(ProjectAssetReference *child)
{
  m_child = child;
}

ProjectAssetReference* ProjectAssetReference::GetChild() const
{
  return m_child;
}

ProjectAssetReference* ProjectAssetReference::InsertChild(ProjectAssetReference *child)
{
  if (!child)
  {
    return this;
  }

  if (child->GetPriority() < GetPriority())
  {
    child->SetChild(this);
    child->SetParent(0);
    SetParent(child);
    return child;
  }
  else
  {
    if (m_child)
    {
      SetChild(m_child->InsertChild(child));
      m_child->SetParent(this);
    }
    else
    {
      SetChild(child);
      child->SetParent(this);
    }
    return this;
  }
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




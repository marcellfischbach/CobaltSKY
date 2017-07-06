#pragma once

#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csstring.hh>

#include <vector>

class ProjectAssetReference;
class ProjectAssetReference
{
public:
  ProjectAssetReference();

  void SetName(const csString &name);
  const csString &GetName() const;

  void SetTypeName(const csString &typeName);
  const csString &GetTypeName() const;

  void SetPriority(unsigned priority);
  unsigned GetPriority() const;

  void SetResourceLocator(const csResourceLocator &locator);
  const csResourceLocator &GetResourceLocator() const;

  void SetParent(ProjectAssetReference *parent);
  ProjectAssetReference *GetParent() const;

  void SetChild(ProjectAssetReference *child);
  ProjectAssetReference* GetChild() const;

  ProjectAssetReference *InsertChild(ProjectAssetReference *child);

  void AddReference(ProjectAssetReference *reference);
  void RemoveReference(ProjectAssetReference *reference);
  void ClearReference();

  void AddReferencedBy(ProjectAssetReference *referencedBy);
  void RemoveReferencedBy(ProjectAssetReference *referencedBy);
  void ClearReferencedBy();

  bool IsReferencing(ProjectAssetReference *reference) const;
  bool IsReferencedBy(ProjectAssetReference *reference) const;

  const std::vector<ProjectAssetReference*> &GetReferences() const;
  const std::vector<ProjectAssetReference*> &GetReferencedBy() const;
private:

  csString m_name;
  csString m_typeName;
  unsigned m_priority;

  csResourceLocator m_resourceLocator;
  ProjectAssetReference *m_parent;
  ProjectAssetReference *m_child;
  std::vector<ProjectAssetReference*> m_references;
  std::vector<ProjectAssetReference*> m_referencedBy;
};

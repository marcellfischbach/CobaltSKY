
#pragma once

#include <cobalt/core/csresourcelocator.hh>

#include <map>
#include <set>
#include <string>
#include <vector>
#include <string>

class ProjectAssetReference;
class ProjectReferenceTree
{
public:
  ProjectReferenceTree();
  ~ProjectReferenceTree();

  void Open(const std::string &projectPath);
  void Close();

  void Rebuild();

  const ProjectAssetReference *GetResource(const csResourceLocator &locator);
  void UpdateDependencyTree(const csResourceLocator &resourceLocator);
  void Rename(const csResourceLocator &from, const csResourceLocator &to);

  const ProjectAssetReference *GetReference(const csResourceLocator &locator) const;

  void Debug();
private:
  void Clear();
  void LoadReferenceTree();
  void StoreReferenceTree();
  void RebuildTree();
  void Debug(ProjectAssetReference *reference, int indent);

private:
  std::string m_projectPath;
  std::map<csResourceLocator, ProjectAssetReference*> m_references;
  std::map<csResourceLocator, ProjectAssetReference*> m_allReferences;
};

#pragma once

#include <map>
#include <set>
#include <string>
#include <cobalt/core/csstring.hh>
#include <cobalt/core/csresourcelocator.hh>

class ProjectAssetReference;
class ProjectReferenceTree
{
public:
  ProjectReferenceTree();

  void Open(const std::string &projectPath);
  void Close();

  void Rebuild();

  const ProjectAssetReference *GetResource(const csResourceLocator &locator);
  void UpdateDependencyTree(const csResourceLocator &resourceLocator);
  void Rename(const csResourceLocator &from, const csResourceLocator &to);

  const ProjectAssetReference *GetReference(const csResourceLocator &locator) const;
private:
  void Clear();
  void LoadReferenceTree();
  void StoreReferenceTree();

private:
  std::string m_projectPath;
  std::map<csResourceLocator, ProjectAssetReference*> m_references;
};
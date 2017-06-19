
#pragma once

#include <map>
#include <set>
#include <string>
#include <valkyrie/core/csstring.hh>
#include <valkyrie/core/csresourcelocator.hh>

class ProjectReferenceTree
{
public:
  ProjectReferenceTree();

  void Open(const std::string &projectPath);
  void Close();

  void RebuildDependencyTree();
  void UpdateDependencyTree(const csResourceLocator &resourceName);

  void Rename(const csResourceLocator &from, const csResourceLocator &to);
  std::set<csResourceLocator> GetReference(const csResourceLocator &resource) const;
  std::set<csResourceLocator> GetReferencedBy(const csResourceLocator &resource) const;


private:
  void LoadDependencyTree();
  void StoreDependencyTree();
  void Rename(std::map<csResourceLocator, std::set<csResourceLocator>> &references, const csResourceLocator &from, const csResourceLocator &to);
  std::string m_projectPath;

  std::set<csResourceLocator> m_allResources;
  std::map<csResourceLocator, std::set<csResourceLocator>> m_references;
  std::map<csResourceLocator, std::set<csResourceLocator>> m_referencedBy;
};
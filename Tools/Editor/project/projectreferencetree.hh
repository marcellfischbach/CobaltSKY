
#pragma once

#include <map>
#include <set>
#include <string>
#include <valkyrie/core/vkstring.hh>
#include <valkyrie/core/vkresourcelocator.hh>

class ProjectReferenceTree
{
public:
  ProjectReferenceTree();

  void Open(const std::string &projectPath);
  void Close();

  void RebuildDependencyTree();
  void UpdateDependencyTree(const vkResourceLocator &resourceName);

  void Rename(const vkResourceLocator &from, const vkResourceLocator &to);
  std::set<vkResourceLocator> GetReference(const vkResourceLocator &resource) const;
  std::set<vkResourceLocator> GetReferencedBy(const vkResourceLocator &resource) const;


private:
  void LoadDependencyTree();
  void StoreDependencyTree();
  void Rename(std::map<vkResourceLocator, std::set<vkResourceLocator>> &references, const vkResourceLocator &from, const vkResourceLocator &to);
  std::string m_projectPath;

  std::set<vkResourceLocator> m_allResources;
  std::map<vkResourceLocator, std::set<vkResourceLocator>> m_references;
  std::map<vkResourceLocator, std::set<vkResourceLocator>> m_referencedBy;
};
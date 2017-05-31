
#include <map>
#include <set>
#include <string>
#include <valkyrie/core/vkstring.hh>

class ProjectDependencyTree
{
public:
  ProjectDependencyTree();

  void Open(const std::string &projectPath);
  void Close();

  void RebuildDependencyTree();
  void UpdateDependencyTree(const std::string &resourceName);

private:
  void LoadDependencyTree();
  void StoreDependencyTree();
  std::string m_projectPath;

  std::set<std::string> m_allResources;
  std::map<std::string, std::set<std::string>> m_references;
  std::map<std::string, std::set<std::string>> m_referenceDependencies;
};
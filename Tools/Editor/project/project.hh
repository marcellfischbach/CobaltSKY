#pragma once

#include <valkyrie/core/vkstring.hh>
#include <project/projectdependencytree.hh>


class Project
{
public:
  Project();

  void Open(const std::string &projectPath);
  void Close();

  ProjectDependencyTree &GetDependencyTree();
  const ProjectDependencyTree &GetDependencyTree() const;

private:
  ProjectDependencyTree m_dependencyTree;
};
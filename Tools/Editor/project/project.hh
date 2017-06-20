#pragma once

#include <cobalt/core/csstring.hh>
#include <project/projectreferencetree.hh>


class Project
{
public:
  Project();

  void Open(const std::string &projectPath);
  void Close();

  ProjectReferenceTree &GetDependencyTree();
  const ProjectReferenceTree &GetDependencyTree() const;

private:
  ProjectReferenceTree m_dependencyTree;
};
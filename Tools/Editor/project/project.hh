#pragma once

#include <cobalt/core/csstring.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <project/projectreferencetree.hh>
#include <QObject>

class Project : public QObject
{
  Q_OBJECT;
public:
  Project();

  void Open(const std::string &projectPath);
  void Close();

  ProjectReferenceTree &GetDependencyTree();
  const ProjectReferenceTree &GetDependencyTree() const;

  bool Rename(const csResourceLocator &from, const csResourceLocator &to);

signals:
  void ResourceRenamed(const csResourceLocator &from, const csResourceLocator &to);

private:
  ProjectReferenceTree m_dependencyTree;
};
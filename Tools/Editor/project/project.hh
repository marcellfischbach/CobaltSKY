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

  ProjectReferenceTree &GetReferenceTree();
  const ProjectReferenceTree &GetReferenceTree() const;

  bool Rename(const csResourceLocator &from, const csResourceLocator &to);


private:
  ProjectReferenceTree m_referenceTree;
};
#pragma once

#include <editor/editorexport.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <editor/project/projectreferencetree.hh>
#include <string>
#include <QObject>

namespace asset::model
{
  class Model;
}

class Project : public QObject
{
  Q_OBJECT;
public:
  Project();

  void Open(const std::string &projectPath);
  void Close();

  ProjectReferenceTree &GetReferenceTree();
  const ProjectReferenceTree &GetReferenceTree() const;

  const asset::model::Model* GetModel() const;
  asset::model::Model* GetModel();

  bool Rename(const csResourceLocator &from, const csResourceLocator &to);


private:
  ProjectReferenceTree m_referenceTree;

  asset::model::Model* m_model;

};


inline const asset::model::Model *Project::GetModel() const
{
  return m_model;
}

inline asset::model::Model *Project::GetModel() 
{
  return m_model;
}

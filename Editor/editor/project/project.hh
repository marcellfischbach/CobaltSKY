#pragma once

#include <editor/editorexport.hh>
#include <cobalt/core/csresourcelocator.hh>
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

  const asset::model::Model* GetModel() const;
  asset::model::Model* GetModel();

private slots:
  void ResourceChanged(const cs::ResourceLocator &locator);

private:

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

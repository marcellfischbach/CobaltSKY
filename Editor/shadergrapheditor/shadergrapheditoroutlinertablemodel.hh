#pragma once

#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <QAbstractItemModel>

namespace cs
{
class SGShaderGraph;
class SGNode;
}

class ShaderGraphEditorOutlinerTableModel : public QAbstractItemModel
{
public:
  ShaderGraphEditorOutlinerTableModel();
  virtual ~ShaderGraphEditorOutlinerTableModel();

  void SetShaderGraph(cs::SGShaderGraph *shaderGraph);

  void NodeChanged(cs::SGNode *node);
  void NodeAdded(cs::SGNode *node);
  void NodeAboutToAdd(cs::SGNode *node);
  void NodeRemoved(cs::SGNode *node);
  void NodeAboutToRemove(cs::SGNode *node);


  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

  QModelIndex GetIndex(cs::SGShaderGraph *graph, int column);
  QModelIndex GetIndex(cs::SGNode *node, int column);

private:
  cs::SGShaderGraph *m_shaderGraph;
};


#pragma once

#include <QAbstractItemModel>

class csSGShaderGraph;
class csSGNode;
class ShaderGraphEditorOutlinerTableModel : public QAbstractItemModel
{
public:
  ShaderGraphEditorOutlinerTableModel();
  virtual ~ShaderGraphEditorOutlinerTableModel();

  void SetShaderGraph(csSGShaderGraph *shaderGraph);

  void NodeChanged(csSGNode *node);
  void NodeAdded(csSGNode *node);
  void NodeAboutToAdd(csSGNode *node);
  void NodeRemoved(csSGNode *node);
  void NodeAboutToRemove(csSGNode *node);


  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

  QModelIndex GetIndex(csSGShaderGraph *graph, int column);
  QModelIndex GetIndex(csSGNode *node, int column);

private:
  csSGShaderGraph *m_shaderGraph;
};


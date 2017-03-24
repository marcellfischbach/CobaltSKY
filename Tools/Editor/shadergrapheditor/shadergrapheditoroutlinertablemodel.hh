#pragma once

#include <QAbstractItemModel>

class vkSGShaderGraph;
class vkSGNode;
class ShaderGraphEditorOutlinerTableModel : public QAbstractItemModel
{
public:
  ShaderGraphEditorOutlinerTableModel();
  virtual ~ShaderGraphEditorOutlinerTableModel();

  void SetShaderGraph(vkSGShaderGraph *shaderGraph);

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

  QModelIndex GetIndex(vkSGShaderGraph *graph, int column);
  QModelIndex GetIndex(vkSGNode *node, int column);

private:
  vkSGShaderGraph *m_shaderGraph;
};


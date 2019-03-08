

#pragma once

#include <assetmanager/assetmanagerexport.hh>
#include <QAbstractItemModel>
#include <map>

namespace cs::editor::model
{
class Model;
class Node;
}

namespace cs::editor::assetmanager
{

class TreeNode;
class ASSET_MANAGER_API TreeModel : public QAbstractItemModel
{
public:
  TreeModel();

  void SetEditorModel(model::Model *editorModel);

  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &index) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;


private:
  void Cleanup();
  void Initialize();
  TreeNode *CreateTreeNode(const model::Node* node);

  TreeNode *m_rootNode;
  model::Model * m_editorModel;

  std::map<model::Node*, TreeNode*> m_nodes;

};


}
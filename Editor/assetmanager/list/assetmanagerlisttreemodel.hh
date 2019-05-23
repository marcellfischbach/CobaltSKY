

#pragma once

#include <assetmanager/assetmanagerexport.hh>
#include <cobalt/core/cssignalslot.hh>
#include <QAbstractItemModel>
#include <map>


namespace cs
{
class ResourceLocator;

}

namespace cs::editor::model
{
class Model;
class Node;
}

class QTreeView;
namespace cs::editor::assetmanager
{

class ListTreeNode;
class ASSET_MANAGER_API ListTreeModel : public QAbstractItemModel
{
public:
  ListTreeModel(QTreeView* treeView);

  void SetEditorModel(model::Model *editorModel);

  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &index) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

  virtual Qt::ItemFlags	flags(const QModelIndex &index) const;
  virtual QStringList mimeTypes() const;
  virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
  virtual bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;
  virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

  
  ListTreeNode *FindNode(const cs::ResourceLocator &locator);
  const ListTreeNode *FindNode(const cs::ResourceLocator &locator) const;

  ListTreeNode *ListTreeNodeAt(const QModelIndex &index);
  const ListTreeNode *ListTreeNodeAt(const QModelIndex &index) const;

  model::Node *ModelNodeAt(const QModelIndex &index);
  const model::Node *ModelNodeAt(const QModelIndex &index) const;


private:
  QModelIndex IndexOf(ListTreeNode *node, int column = 0) const;

  void OnModelNodeAdded(model::Node* node, model::Node* parent);
  void OnModelNodeMoved(model::Node* node, model::Node* oldParent, model::Node* newParent);
  void OnModelNodeChanged(model::Node* node);
  void OnModelNodeRemoved(model::Node* node, model::Node* oldParent);

  cs::Slot<model::Node*, model::Node*> m_slotNodeAdded;
  cs::Slot<model::Node*, model::Node*, model::Node *> m_slotNodeMoved;
  cs::Slot<model::Node*> m_slotNodeChanged;
  cs::Slot<model::Node*, model::Node*> m_slotNodeRemoved;

  void Cleanup();
  void Initialize();
  ListTreeNode *CreateListTreeNode(model::Node* node);

  ListTreeNode *m_rootNode;
  model::Model * m_editorModel;
  QTreeView* m_treeView;

  std::map<const model::Node*, ListTreeNode*> m_nodes;

};


}


#pragma once

#include <assetmanager/assetmanagerexport.hh>
#include <cobalt/core/cssignalslot.hh>
#include <QAbstractItemModel>
#include <QIcon>
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

class QListView;
namespace cs::editor::assetmanager
{

class ListListNode;
class ASSET_MANAGER_API ListListModel : public QAbstractItemModel
{
public:
  ListListModel(QListView* listView);

  void SetEditorModel(model::Model* editorModel);
  void SetEditorNode(model::Node* editorNode);

  virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex& index) const;
  virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

  /*
  virtual Qt::ItemFlags	flags(const QModelIndex& index) const;
  virtual QStringList TreeModel::mimeTypes() const;
  virtual QMimeData* TreeModel::mimeData(const QModelIndexList& indexes) const;
  virtual bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const;
  virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);
  */

  ListListNode* FindNode(const cs::ResourceLocator& locator);
  const ListListNode* FindNode(const cs::ResourceLocator& locator) const;

  ListListNode* ListListNodeAt(const QModelIndex& index);
  const ListListNode* ListListNodeAt(const QModelIndex& index) const;

  model::Node* ModelNodeAt(const QModelIndex& index);
  const model::Node* ModelNodeAt(const QModelIndex& index) const;


private:
  QModelIndex IndexOf(ListListNode* node, int column = 0) const;

  void OnModelNodeAdded(model::Node* node, model::Node* parent);
  void OnModelNodeMoved(model::Node* node, model::Node* oldParent, model::Node* newParent);
  void OnModelNodeChanged(model::Node* node);
  void OnModelNodeRemoved(model::Node* node, model::Node* oldParent);

  cs::Slot<model::Node*, model::Node*> m_slotNodeAdded;
  cs::Slot<model::Node*, model::Node*, model::Node*> m_slotNodeMoved;
  cs::Slot<model::Node*> m_slotNodeChanged;
  cs::Slot<model::Node*, model::Node*> m_slotNodeRemoved;

  void CleanupModel();
  void CleanupNodes();
  void Initialize();
  ListListNode* CreateListListNode(model::Node* node);

  ListListNode* m_rootNode;
  model::Model* m_editorModel;
  QListView* m_listView;

  std::map<const model::Node*, ListListNode*> m_nodes;

  QIcon m_folderIcon;
  QIcon m_emptyIcon;

};


}
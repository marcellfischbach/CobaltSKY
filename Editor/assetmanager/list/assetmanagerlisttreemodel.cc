
#include <assetmanager/list/assetmanagerlisttreemodel.hh>
#include <assetmanager/list/assetmanagerlisttreenode.hh>
#include <editormodel/model.hh>
#include <editormodel/nodes/node.hh>

#include <cobalt/core/csresourcelocator.hh>

#include <QDataStream>
#include <QMimeData>
#include <QIcon>
#include <QTreeView>

#include <iostream>



namespace cs::editor::assetmanager
{

ListTreeNode* ListTreeNodeFrom(const QModelIndex& index)
{
  return index.isValid() ? reinterpret_cast<ListTreeNode*>(index.internalPointer()) : nullptr;
}

ListTreeModel::ListTreeModel(QTreeView* treeView)
  : m_editorModel(nullptr)
  , m_rootNode(nullptr)
  , m_treeView(treeView)
{

}




int ListTreeModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}

QVariant ListTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole)
  {
    switch (section)
    {
    case 0:
      return QVariant(QObject::tr("Name"));
    case 1:
      return QVariant(QObject::tr("Type"));
    }
  }
  return QAbstractItemModel::headerData(section, orientation, role);
}

QVariant ListTreeModel::data(const QModelIndex& index, int role) const
{
  ListTreeNode* treeNode = ListTreeNodeFrom(index);


  if (treeNode)
  {
    if (role == Qt::DisplayRole)
    {
      switch (index.column())
      {
      case 0:
        return QVariant(treeNode->GetName());
      case 1:
        return QVariant(treeNode->GetType());
      }
    }
    else if (role == Qt::DecorationRole)
    {
      if (treeNode->GetNode()->IsFolderNode())
      {
        if (m_treeView && m_treeView->isExpanded(index))
        {
          return QVariant(QIcon(":/icons16/folder_opened"));
        }
        else
        {
          return QVariant(QIcon(":/icons16/folder_closed"));
        }
      }
    }
  }
  return QVariant();
}
QModelIndex	ListTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  ListTreeNode* parentListTreeNode = ListTreeNodeFrom(parent);
  if (parentListTreeNode)
  {
    if (row >= 0 && row < parentListTreeNode->m_children.size())
    {
      return createIndex(row, column, parentListTreeNode->m_children[row]);
    }
  }
  else if (row == 0)
  {
    return createIndex(row, column, m_rootNode);
  }
  return QModelIndex();
}

QModelIndex	ListTreeModel::parent(const QModelIndex& index) const
{
  ListTreeNode* treeNode = ListTreeNodeFrom(index);
  if (!treeNode)
  {
    return QModelIndex();
  }

  ListTreeNode* parentListTreeNode = treeNode->m_parent;
  if (!parentListTreeNode)
  {
    return QModelIndex();
  }

  return createIndex(parentListTreeNode->GetIndexWithinParent(0), 0, parentListTreeNode);
}

int	ListTreeModel::rowCount(const QModelIndex& parent) const
{
  ListTreeNode* parentListTreeNode = ListTreeNodeFrom(parent);
  if (parentListTreeNode)
  {
    return parentListTreeNode->m_children.size();
  }
  else if (m_rootNode)
  {
    return 1;
  }
  return 0;
}

Qt::ItemFlags	ListTreeModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags fl = QAbstractItemModel::flags(index);

  ListTreeNode* treeNode = ListTreeNodeFrom(index);
  if (treeNode)
  {
    switch (treeNode->GetNode()->GetType())
    {
    case model::Node::eT_Asset:
    case model::Node::eT_Folder:
      fl |= Qt::ItemIsDragEnabled;
      break;
    }
    if (!treeNode->GetNode()->IsRootNode())
    {
      fl |= Qt::ItemIsDropEnabled;
    }
  }
  return fl;
}


QStringList ListTreeModel::mimeTypes() const
{
  QStringList types;
  types << "text/csResourceLocator";
  //types << "text/uri-list";
  return types;
}



QMimeData* ListTreeModel::mimeData(const QModelIndexList& indexes) const
{
  QMimeData* md = new QMimeData();

  QByteArray encodedData;

  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  for (auto idx : indexes)
  {
    if (idx.isValid() && idx.column() == 0)
    {
      ListTreeNode* node = ListTreeNodeFrom(idx);
      stream << QString(node->GetNode()->GetResourceLocator().Encode().c_str());
    }
  }

  md->setData("text/csResourceLocator", encodedData);
  return md;
}

bool ListTreeModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
  if (data->hasFormat("text/csResourceLocator"))
  {

    QByteArray encodedData = data->data("text/csResourceLocator");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QString encodedLocator;
    stream >> encodedLocator;

    csResourceLocator locator(encodedLocator.toLatin1().data());
    const ListTreeNode* dragNode = FindNode(locator);
    ListTreeNode* dropNode = ListTreeNodeFrom(parent);
    if (row != -1 || column != -1)
    {
      return false;
    }

    if (!dragNode || !dropNode)
    {
      return false;
    }


    if (!dropNode->GetNode()->IsFolderNode())
    {
      return false;
    }
    if (dropNode->HasParent(dragNode))
    {
      return false;
    }
  }

  return true;
}

bool ListTreeModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
  if (!canDropMimeData(data, action, row, column, parent))
  {
    return true;
  }

  if (data->hasFormat("text/csResourceLocator"))
  {

    QByteArray encodedData = data->data("text/csResourceLocator");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QString encodedLocator;
    stream >> encodedLocator;

    csResourceLocator locator(encodedLocator.toLatin1().data());
    ListTreeNode* dragNode = FindNode(locator);
    ListTreeNode* dropNode = ListTreeNodeFrom(parent);


    model::Transaction tx = m_editorModel->CreateTransaction();
    try
    {
      tx.Begin();
    }
    catch (std::exception & e)
    {
      // cannot even begin a transaction... there something real foul
      return true;
    }

    try
    {

      m_editorModel->Move(dragNode->GetNode(), dropNode->GetNode()->AsFolderNode(), tx);
      tx.Commit();

    }
    catch (std::exception & e)
    {
      tx.Rollback();
    }
  }

  return true;
}

ListTreeNode* ListTreeModel::FindNode(const csResourceLocator& locator)
{
  return const_cast<ListTreeNode*>(
    static_cast<const ListTreeModel*>(this)->FindNode(locator)
    );
}

const ListTreeNode* ListTreeModel::FindNode(const csResourceLocator& locator) const
{
  for (auto e : m_nodes)
  {
    if (e.first->GetResourceLocator() == locator)
    {
      return e.second;
    }
  }
  return nullptr;
}


ListTreeNode* ListTreeModel::ListTreeNodeAt(const QModelIndex& index)
{
  return const_cast<ListTreeNode*>(
    static_cast<const ListTreeModel*>(this)->ListTreeNodeAt(index)
    );
}

const ListTreeNode* ListTreeModel::ListTreeNodeAt(const QModelIndex& index) const
{
  ListTreeNode* node = ListTreeNodeFrom(index);
  return node;
}



model::Node* ListTreeModel::ModelNodeAt(const QModelIndex& index)
{
  return const_cast<model::Node*>(
    static_cast<const ListTreeModel*>(this)->ModelNodeAt(index)
    );
}

const model::Node* ListTreeModel::ModelNodeAt(const QModelIndex& index) const
{
  const ListTreeNode* node = ListTreeNodeAt(index);

  return node ? node->m_node : nullptr;
}


void ListTreeModel::SetEditorModel(model::Model* editorModel)
{
  Cleanup();
  m_editorModel = editorModel;

  if (m_editorModel)
  {
    Initialize();
  }
}

void ListTreeModel::Cleanup()
{
  if (m_editorModel)
  {
    m_slotNodeAdded.Disconnect();
    m_slotNodeMoved.Disconnect();
    m_slotNodeChanged.Disconnect();
    m_slotNodeRemoved.Disconnect();
  }
  for (auto e : m_nodes)
  {
    delete e.second;
  }
  m_nodes.clear();
  m_rootNode = nullptr;
}

void ListTreeModel::Initialize()
{
  m_slotNodeAdded = m_editorModel->OnTreeStructNodeAdded().Connect([this](model::Node * child, model::Node * parent) { OnModelNodeAdded(child, parent); });
  m_slotNodeChanged = m_editorModel->OnTreeStructNodeChanged().Connect([this](model::Node * node) { OnModelNodeChanged(node); });
  m_slotNodeMoved = m_editorModel->OnTreeStructNodeMoved().Connect([this](model::Node * node, model::Node * oldParent, model::Node * newParent) { OnModelNodeMoved(node, oldParent, newParent); });
  m_slotNodeRemoved = m_editorModel->OnTreeStructNodeRemoved().Connect([this](model::Node * child, model::Node * oldParent) { OnModelNodeRemoved(child, oldParent); });

  beginResetModel();
  m_rootNode = CreateListTreeNode(m_editorModel->GetRoot());
  endResetModel();
}

ListTreeNode* ListTreeModel::CreateListTreeNode(model::Node* node)
{
  if (node->IsAssetNode())
  {
    return nullptr;
  }

  ListTreeNode* treeNode = new ListTreeNode(node);
  m_nodes[node] = treeNode;
  for (auto child : static_cast<const model::Node*>(node)->GetChildren())
  {
    ListTreeNode* childNode = CreateListTreeNode(child);
    if (childNode)
    {
      treeNode->m_children.push_back(childNode);
      childNode->m_parent = treeNode;
    }
  }
  treeNode->Sort();

  return treeNode;
}


QModelIndex ListTreeModel::IndexOf(ListTreeNode* node, int column) const
{
  if (!node)
  {
    return QModelIndex();
  }
  return createIndex(node->GetIndexWithinParent(), column, node);
}




void ListTreeModel::OnModelNodeAdded(model::Node* node, model::Node* parent)
{
  ListTreeNode* treeNode = CreateListTreeNode(node);
  if (treeNode == nullptr)
  {
    return;
  }

  auto it = m_nodes.find(parent);
  if (it == m_nodes.end())
  {
    delete treeNode;
    return;
  }
  ListTreeNode* parentNode = it->second;
  parentNode->m_children.push_back(treeNode);
  treeNode->m_parent = parentNode;
  parentNode->Sort();
  int idx = parentNode->GetIndexOfChild(treeNode);
  beginInsertRows(IndexOf(parentNode), idx, idx);
  endInsertRows();
}

void ListTreeModel::OnModelNodeMoved(model::Node* node, model::Node* oldParent, model::Node* newParent)
{
  auto it = m_nodes.find(node);
  ListTreeNode* treeNode = (it != m_nodes.end() ? it->second : nullptr);

  it = m_nodes.find(oldParent);
  ListTreeNode * treeOldParent = (it != m_nodes.end() ? it->second : nullptr);

  it = m_nodes.find(newParent);
  ListTreeNode * treeNewParent = (it != m_nodes.end() ? it->second : nullptr);


  if (!treeNode || !treeOldParent || !treeNewParent)
  {
    return;
  }

  if (treeNode->m_parent != treeOldParent)
  {
    return;
  }

  int oldIdx = treeOldParent->GetIndexOfChild(treeNode);
  if (oldIdx != -1)
  {
    beginRemoveRows(IndexOf(treeOldParent), oldIdx, oldIdx);
    auto tnIt = std::find(treeOldParent->m_children.begin(), treeOldParent->m_children.end(), treeNode);
    if (tnIt != treeOldParent->m_children.end())
    {
      treeOldParent->m_children.erase(tnIt);
    }
    endRemoveRows();
  }

  treeNewParent->m_children.push_back(treeNode);
  treeNode->m_parent = treeNewParent;
  treeNewParent->Sort();

  int newIdx = treeNewParent->GetIndexOfChild(treeNode);
  beginInsertRows(IndexOf(treeNewParent), newIdx, newIdx);
  endInsertRows();
}

void ListTreeModel::OnModelNodeChanged(model::Node* node)
{

}

void ListTreeModel::OnModelNodeRemoved(model::Node* node, model::Node* oldParent)
{
  auto it = m_nodes.find(node);
  ListTreeNode* treeNode = (it != m_nodes.end() ? it->second : nullptr);
  it = m_nodes.find(oldParent);
  ListTreeNode * treeOldParent = (it != m_nodes.end() ? it->second : nullptr);

  if (!treeNode || !oldParent)
  {
    return;
  }

  int oldIdx = treeOldParent->GetIndexOfChild(treeNode);
  if (oldIdx != -1)
  {
    beginRemoveRows(IndexOf(treeOldParent), oldIdx, oldIdx);
    auto tnIt = std::find(treeOldParent->m_children.begin(), treeOldParent->m_children.end(), treeNode);
    if (tnIt != treeOldParent->m_children.end())
    {
      treeOldParent->m_children.erase(tnIt);
    }
    endRemoveRows();
  }
  delete treeNode;
}

}
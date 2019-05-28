
#include <assetmanager/list/assetmanagerlistlistmodel.hh>
#include <assetmanager/list/assetmanagerlistlistnode.hh>
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

ListListNode* ListListNodeFrom(const QModelIndex& index)
{
  return index.isValid() ? reinterpret_cast<ListListNode*>(index.internalPointer()) : nullptr;
}

ListListModel::ListListModel(QListView* listView)
  : m_editorModel(nullptr)
  , m_rootNode(nullptr)
  , m_listView(listView)
  , m_folderIcon(QIcon(":/icons32/folder_closed"))
{
  QPixmap pm(64, 64);
  pm.fill(QColor(0, 0, 0, 0));
  m_emptyIcon = QIcon(pm);
}




int ListListModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}


QVariant ListListModel::data(const QModelIndex& index, int role) const
{
  ListListNode* listNode = ListListNodeFrom(index);


  if (listNode)
  {
    if (role == Qt::DisplayRole)
    {
      return QVariant(listNode->GetName());
    }
    else if (role == Qt::DecorationRole)
    {
      if (listNode->GetNode()->IsFolderNode())
      {
        return QVariant(m_folderIcon);
      }
      else
      {
        return QVariant(m_emptyIcon);
      }
    }
  }
  return QVariant();
}
QModelIndex	ListListModel::index(int row, int column, const QModelIndex& parent) const
{
  if (parent.isValid() || !m_rootNode)
  {
    return QModelIndex();
  }

  if (row >= 0 && row < m_rootNode->m_children.size())
  {
    return createIndex(row, column, m_rootNode->m_children[row]);
  }

  return QModelIndex();
}

QModelIndex	ListListModel::parent(const QModelIndex& index) const
{
  return QModelIndex();
}

int	ListListModel::rowCount(const QModelIndex& parent) const
{
  int count = 0;
  if (!parent.isValid() && m_rootNode)
  {
    count = m_rootNode->GetNumberOfChildren();
  }
  return count;
}

/*

Qt::ItemFlags	ListListModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags fl = QAbstractItemModel::flags(index);

  ListListNode* listNode = ListListNodeFrom(index);
  if (listNode)
  {
    switch (listNode->GetNode()->GetType())
    {
    case model::Node::eT_Asset:
    case model::Node::eT_Folder:
      fl |= Qt::ItemIsDragEnabled;
      break;
    }
    if (!listNode->GetNode()->IsRootNode())
    {
      fl |= Qt::ItemIsDropEnabled;
    }
  }
  return fl;
}


QStringList ListListModel::mimeTypes() const
{
  QStringList types;
  types << "text/cs::ResourceLocator";
  //types << "text/uri-list";
  return types;
}



QMimeData* ListListModel::mimeData(const QModelIndexList& indexes) const
{
  QMimeData* md = new QMimeData();

  QByteArray encodedData;

  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  for (auto idx : indexes)
  {
    if (idx.isValid() && idx.column() == 0)
    {
      ListListNode* node = ListListNodeFrom(idx);
      stream << QString(node->GetNode()->GetResourceLocator().Encode().c_str());
    }
  }

  md->setData("text/cs::ResourceLocator", encodedData);
  return md;
}

bool ListListModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
  if (data->hasFormat("text/cs::ResourceLocator"))
  {

    QByteArray encodedData = data->data("text/cs::ResourceLocator");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QString encodedLocator;
    stream >> encodedLocator;

    cs::ResourceLocator locator(encodedLocator.toLatin1().data());
    const ListListNode* dragNode = FindNode(locator);
    ListListNode* dropNode = ListListNodeFrom(parent);
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

bool ListListModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
  if (!canDropMimeData(data, action, row, column, parent))
  {
    return true;
  }

  if (data->hasFormat("text/cs::ResourceLocator"))
  {

    QByteArray encodedData = data->data("text/cs::ResourceLocator");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QString encodedLocator;
    stream >> encodedLocator;

    cs::ResourceLocator locator(encodedLocator.toLatin1().data());
    ListListNode* dragNode = FindNode(locator);
    ListListNode* dropNode = ListListNodeFrom(parent);


    model::Transaction tx = m_editorModel->CreateTransaction();
    try
    {
      tx.Begin();
    }
    catch (std::exception & e)
    {
      std::cerr << "Unable to begin transaction. Cause '" << e.what() << "'\n";
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
      std::cerr << "Unexpected error. Cause '" << e.what() << "'\n";
      tx.Rollback();
    }
  }

  return true;
}

*/

ListListNode* ListListModel::FindNode(const cs::ResourceLocator& locator)
{
  return const_cast<ListListNode*>(
    static_cast<const ListListModel*>(this)->FindNode(locator)
    );
}

const ListListNode* ListListModel::FindNode(const cs::ResourceLocator& locator) const
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


ListListNode* ListListModel::ListListNodeAt(const QModelIndex& index)
{
  return const_cast<ListListNode*>(
    static_cast<const ListListModel*>(this)->ListListNodeAt(index)
    );
}

const ListListNode* ListListModel::ListListNodeAt(const QModelIndex& index) const
{
  ListListNode* node = ListListNodeFrom(index);
  return node;
}



model::Node* ListListModel::ModelNodeAt(const QModelIndex& index)
{
  return const_cast<model::Node*>(
    static_cast<const ListListModel*>(this)->ModelNodeAt(index)
    );
}

const model::Node* ListListModel::ModelNodeAt(const QModelIndex& index) const
{
  const ListListNode* node = ListListNodeAt(index);

  return node ? node->m_node : nullptr;
}


void ListListModel::SetEditorModel(model::Model* editorModel)
{
  CleanupModel();
  m_editorModel = editorModel;

  if (m_editorModel)
  {
    Initialize();
  }
}

void ListListModel::SetEditorNode(model::Node* editorNode)
{
  CleanupNodes();

  beginResetModel();
  if (editorNode)
  {
    m_rootNode = CreateListListNode(editorNode);
    for (auto child : static_cast<const model::Node*>(editorNode)->GetChildren())
    {
      ListListNode* childNode = CreateListListNode(child);
      if (childNode)
      {
        m_rootNode->m_children.push_back(childNode);
        childNode->m_parent = m_rootNode;
      }
    }
    m_rootNode->Sort();
  }
  endResetModel();
}

void ListListModel::CleanupModel()
{
  if (m_editorModel)
  {
    m_slotNodeAdded.Disconnect();
    m_slotNodeMoved.Disconnect();
    m_slotNodeChanged.Disconnect();
    m_slotNodeRemoved.Disconnect();
  }

}

void ListListModel::CleanupNodes()
{
  delete m_rootNode;
  m_nodes.clear();
  m_rootNode = nullptr;
}

void ListListModel::Initialize()
{
  m_slotNodeAdded = m_editorModel->OnTreeStructNodeAdded().Connect([this](model::Node * child, model::Node * parent) { OnModelNodeAdded(child, parent); });
  m_slotNodeChanged = m_editorModel->OnTreeStructNodeChanged().Connect([this](model::Node * node) { OnModelNodeChanged(node); });
  m_slotNodeMoved = m_editorModel->OnTreeStructNodeMoved().Connect([this](model::Node * node, model::Node * oldParent, model::Node * newParent) { OnModelNodeMoved(node, oldParent, newParent); });
  m_slotNodeRemoved = m_editorModel->OnTreeStructNodeRemoved().Connect([this](model::Node * child, model::Node * oldParent) { OnModelNodeRemoved(child, oldParent); });


}

ListListNode* ListListModel::CreateListListNode(model::Node* node)
{
  ListListNode* listNode = new ListListNode(node);
  m_nodes[node] = listNode;
  return listNode;
}


QModelIndex ListListModel::IndexOf(ListListNode* node, int column) const
{
  if (!node)
  {
    return QModelIndex();
  }
  return createIndex(node->GetIndexWithinParent(), column, node);
}




void ListListModel::OnModelNodeAdded(model::Node* node, model::Node* parent)
{
  if (!m_rootNode || m_rootNode->GetNode() != parent)
  {
    return;
  }

  ListListNode* listNode = CreateListListNode(node);
  m_rootNode->m_children.push_back(listNode);
  listNode->m_parent = m_rootNode;
  m_rootNode->Sort();

  int idx = m_rootNode->GetIndexOfChild(listNode);
  beginInsertRows(QModelIndex(), idx, idx);
  endInsertRows();
}


void ListListModel::OnModelNodeRemoved(model::Node* node, model::Node* oldParent)
{
  if (!m_rootNode || m_rootNode->GetNode() != oldParent)
  {
    return;
  }

  auto it = m_nodes.find(node);
  ListListNode* listNode = (it != m_nodes.end() ? it->second : nullptr);

  if (!listNode)
  {
    return;
  }

  int oldIdx = m_rootNode->GetIndexOfChild(listNode);
  if (oldIdx != -1)
  {
    beginRemoveRows(QModelIndex(), oldIdx, oldIdx);
    auto tnIt = std::find(m_rootNode->m_children.begin(), m_rootNode->m_children.end(), listNode);
    if (tnIt != m_rootNode->m_children.end())
    {
      m_rootNode->m_children.erase(tnIt);
    }
    endRemoveRows();
  }
  delete listNode;
}

void ListListModel::OnModelNodeMoved(model::Node* node, model::Node* oldParent, model::Node* newParent)
{
  if (!m_rootNode)
  {
    return;
  }

  if (m_rootNode->GetNode() == oldParent)
  {
    OnModelNodeRemoved(node, oldParent);
  }
  else if (m_rootNode->GetNode() == newParent)
  {
    OnModelNodeAdded(node, newParent);
  }
}

void ListListModel::OnModelNodeChanged(model::Node* node)
{

}

}
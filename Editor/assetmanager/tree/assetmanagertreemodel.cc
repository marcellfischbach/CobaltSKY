
#include <assetmanager/tree/assetmanagertreemodel.hh>
#include <assetmanager/tree/assetmanagertreenode.hh>
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

TreeNode* TreeNodeFrom(const QModelIndex& index)
{
  return index.isValid() ? reinterpret_cast<TreeNode*>(index.internalPointer()) : nullptr;
}

TreeModel::TreeModel(QTreeView* treeView)
  : m_editorModel(nullptr)
  , m_rootNode(nullptr)
  , m_treeView(treeView)
{

}




int TreeModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant TreeModel::data(const QModelIndex& index, int role) const
{
  TreeNode* treeNode = TreeNodeFrom(index);


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
QModelIndex	TreeModel::index(int row, int column, const QModelIndex& parent) const
{
  TreeNode* parentTreeNode = TreeNodeFrom(parent);
  if (parentTreeNode)
  {
    if (row >= 0 && row < parentTreeNode->m_children.size())
    {
      return createIndex(row, column, parentTreeNode->m_children[row]);
    }
  }
  else if (row == 0)
  {
    return createIndex(row, column, m_rootNode);
  }
  return QModelIndex();
}

QModelIndex	TreeModel::parent(const QModelIndex& index) const
{
  TreeNode* treeNode = TreeNodeFrom(index);
  if (!treeNode)
  {
    return QModelIndex();
  }

  TreeNode* parentTreeNode = treeNode->m_parent;
  if (!parentTreeNode)
  {
    return QModelIndex();
  }

  return createIndex(parentTreeNode->GetIndexWithinParent(0), 0, parentTreeNode);
}

int	TreeModel::rowCount(const QModelIndex& parent) const
{
  TreeNode* parentTreeNode = TreeNodeFrom(parent);
  if (parentTreeNode)
  {
    return parentTreeNode->m_children.size();
  }
  else if (m_rootNode)
  {
    return 1;
  }
  return 0;
}

Qt::ItemFlags	TreeModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags fl = QAbstractItemModel::flags(index);

  TreeNode* treeNode = TreeNodeFrom(index);
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


QStringList TreeModel::mimeTypes() const
{
  QStringList types;
  types << "text/cs::ResourceLocator";
  //types << "text/uri-list";
  return types;
}



QMimeData* TreeModel::mimeData(const QModelIndexList& indexes) const
{
  QMimeData* md = new QMimeData();

  QByteArray encodedData;

  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  for (auto idx : indexes)
  {
    if (idx.isValid() && idx.column() == 0)
    {
      TreeNode* node = TreeNodeFrom(idx);
      stream << QString(node->GetNode()->GetResourceLocator().Encode().c_str());
    }
  }

  md->setData("text/cs::ResourceLocator", encodedData);
  return md;
}

bool TreeModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
  if (data->hasFormat("text/cs::ResourceLocator"))
  {

    QByteArray encodedData = data->data("text/cs::ResourceLocator");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QString encodedLocator;
    stream >> encodedLocator;

    cs::ResourceLocator locator(encodedLocator.toLatin1().data());
    const TreeNode* dragNode = FindNode(locator);
    TreeNode* dropNode = TreeNodeFrom(parent);
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

bool TreeModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
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
    TreeNode* dragNode = FindNode(locator);
    TreeNode* dropNode = TreeNodeFrom(parent);


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

TreeNode* TreeModel::FindNode(const cs::ResourceLocator& locator)
{
  return const_cast<TreeNode*>(
    static_cast<const TreeModel*>(this)->FindNode(locator)
    );
}

const TreeNode* TreeModel::FindNode(const cs::ResourceLocator& locator) const
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


TreeNode* TreeModel::TreeNodeAt(const QModelIndex& index)
{
  return const_cast<TreeNode*>(
    static_cast<const TreeModel*>(this)->TreeNodeAt(index)
    );
}

const TreeNode* TreeModel::TreeNodeAt(const QModelIndex& index) const
{
  TreeNode* node = TreeNodeFrom(index);
  return node;
}



model::Node* TreeModel::ModelNodeAt(const QModelIndex& index)
{
  return const_cast<model::Node*>(
    static_cast<const TreeModel*>(this)->ModelNodeAt(index)
    );
}

const model::Node* TreeModel::ModelNodeAt(const QModelIndex& index) const
{
  const TreeNode* node = TreeNodeAt(index);

  return node ? node->m_node : nullptr;
}


void TreeModel::SetEditorModel(model::Model* editorModel)
{
  Cleanup();
  m_editorModel = editorModel;

  if (m_editorModel)
  {
    Initialize();
  }
}

void TreeModel::Cleanup()
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

void TreeModel::Initialize()
{
  m_slotNodeAdded = m_editorModel->OnTreeStructNodeAdded().Connect([this](model::Node * child, model::Node * parent) { OnModelNodeAdded(child, parent); });
  m_slotNodeChanged = m_editorModel->OnTreeStructNodeChanged().Connect([this](model::Node * node) { OnModelNodeChanged(node); });
  m_slotNodeMoved = m_editorModel->OnTreeStructNodeMoved().Connect([this](model::Node * node, model::Node * oldParent, model::Node * newParent) { OnModelNodeMoved(node, oldParent, newParent); });
  m_slotNodeRemoved = m_editorModel->OnTreeStructNodeRemoved().Connect([this](model::Node * child, model::Node * oldParent) { OnModelNodeRemoved(child, oldParent); });

  beginResetModel();
  m_rootNode = CreateTreeNode(m_editorModel->GetRoot());
  endResetModel();
}

TreeNode* TreeModel::CreateTreeNode(model::Node* node)
{
  TreeNode* treeNode = new TreeNode(node);
  m_nodes[node] = treeNode;
  for (auto child : static_cast<const model::Node*>(node)->GetChildren())
  {
    TreeNode* childNode = CreateTreeNode(child);
    if (childNode)
    {
      treeNode->m_children.push_back(childNode);
      childNode->m_parent = treeNode;
    }
  }
  treeNode->Sort();

  return treeNode;
}


QModelIndex TreeModel::IndexOf(TreeNode* node, int column) const
{
  if (!node)
  {
    return QModelIndex();
  }
  return createIndex(node->GetIndexWithinParent(), column, node);
}




void TreeModel::OnModelNodeAdded(model::Node* node, model::Node* parent)
{
  TreeNode* treeNode = CreateTreeNode(node);
  auto it = m_nodes.find(parent);
  if (it == m_nodes.end())
  {
    return;
  }
  TreeNode* parentNode = it->second;
  parentNode->m_children.push_back(treeNode);
  treeNode->m_parent = parentNode;
  parentNode->Sort();
  int idx = parentNode->GetIndexOfChild(treeNode);
  beginInsertRows(IndexOf(parentNode), idx, idx);
  endInsertRows();
}

void TreeModel::OnModelNodeMoved(model::Node* node, model::Node* oldParent, model::Node* newParent)
{
  auto it = m_nodes.find(node);
  TreeNode* treeNode = (it != m_nodes.end() ? it->second : nullptr);

  it = m_nodes.find(oldParent);
  TreeNode * treeOldParent = (it != m_nodes.end() ? it->second : nullptr);

  it = m_nodes.find(newParent);
  TreeNode * treeNewParent = (it != m_nodes.end() ? it->second : nullptr);


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

void TreeModel::OnModelNodeChanged(model::Node* node)
{

}

void TreeModel::OnModelNodeRemoved(model::Node* node, model::Node* oldParent)
{
  auto it = m_nodes.find(node);
  TreeNode* treeNode = (it != m_nodes.end() ? it->second : nullptr);
  it = m_nodes.find(oldParent);
  TreeNode * treeOldParent = (it != m_nodes.end() ? it->second : nullptr);

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
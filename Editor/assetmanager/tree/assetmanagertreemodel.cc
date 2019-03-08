
#include <assetmanager/tree/assetmanagertreemodel.hh>
#include <assetmanager/tree/assetmanagertreenode.hh>
#include <editormodel/model.hh>
#include <editormodel/nodes/node.hh>



namespace cs::editor::assetmanager
{

TreeNode *TreeNodeFrom(const QModelIndex &index)
{
  return index.isValid() ? reinterpret_cast<TreeNode*>(index.internalPointer()) : nullptr;
}

TreeModel::TreeModel()
  : m_editorModel(nullptr)
  , m_rootNode(nullptr)
{

}



int TreeModel::columnCount(const QModelIndex &parent) const
{
  return 2;
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

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
  TreeNode *treeNode = TreeNodeFrom(index);

  if (treeNode && role == Qt::DisplayRole)
  {
    switch (index.column())
    {
    case 0:
      return QVariant(treeNode->GetName());
    case 1:
      return QVariant(treeNode->GetType());
    }
  }
  return QVariant();
}
QModelIndex	TreeModel::index(int row, int column, const QModelIndex &parent) const
{
  TreeNode *parentTreeNode = TreeNodeFrom(parent);
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

QModelIndex	TreeModel::parent(const QModelIndex &index) const
{
  TreeNode *treeNode = TreeNodeFrom(index);
  if (!treeNode)
  {
    return QModelIndex();
  }

  TreeNode *parentTreeNode = treeNode->m_parent;
  if (!parentTreeNode)
  {
    return QModelIndex();
  }

  return createIndex(parentTreeNode->GetIndexWithinParent(0), 0, parentTreeNode);
}

int	TreeModel::rowCount(const QModelIndex &parent) const
{
  TreeNode *parentTreeNode = TreeNodeFrom(parent);
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


void TreeModel::SetEditorModel(model::Model *editorModel)
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
  for (auto e : m_nodes)
  {
    delete e.second;
  }
  m_nodes.clear();
  m_rootNode = nullptr;
}

void TreeModel::Initialize()
{
  beginResetModel();
  m_rootNode = CreateTreeNode(m_editorModel->GetRoot());
  endResetModel();
}

TreeNode *TreeModel::CreateTreeNode(const model::Node *node)
{
  TreeNode *treeNode = new TreeNode(node);
  for (auto child : node->GetChildren())
  {
    TreeNode* childNode = CreateTreeNode(child);
    if (childNode)
    {
      treeNode->m_children.push_back(childNode);
      childNode->m_parent = treeNode;
    }
  }
  return treeNode;
}

}
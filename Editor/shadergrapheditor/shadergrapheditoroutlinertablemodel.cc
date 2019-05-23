
#include <shadergrapheditor/shadergrapheditoroutlinertablemodel.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>

ShaderGraphEditorOutlinerTableModel::ShaderGraphEditorOutlinerTableModel()
  : QAbstractItemModel()
  , m_shaderGraph(0)
{

}

ShaderGraphEditorOutlinerTableModel::~ShaderGraphEditorOutlinerTableModel()
{

}

void ShaderGraphEditorOutlinerTableModel::SetShaderGraph(cs::SGShaderGraph *shaderGraph)
{
  beginResetModel();
  m_shaderGraph = shaderGraph;

  endResetModel();
}


void ShaderGraphEditorOutlinerTableModel::NodeChanged(cs::SGNode *node)
{
  int idx = m_shaderGraph->GetIndexOfNode(node);
  if (idx == -1)
  {
    return;
  }
  
  emit dataChanged(createIndex(idx, 0, node), createIndex(idx, 1, node));
}

void ShaderGraphEditorOutlinerTableModel::NodeAboutToAdd(cs::SGNode *node)
{

}

void ShaderGraphEditorOutlinerTableModel::NodeAdded(cs::SGNode *node)
{
  int idx = m_shaderGraph->GetIndexOfNode(node);
  if (idx == -1)
  {
    return;
  }

  beginInsertRows(QModelIndex(), idx, idx);
  endInsertRows();
}

void ShaderGraphEditorOutlinerTableModel::NodeAboutToRemove(cs::SGNode *node)
{
  int idx = m_shaderGraph->GetIndexOfNode(node);
  if (idx == -1)
  {
    return;
  }
  beginRemoveRows(QModelIndex(), idx, idx);
}

void ShaderGraphEditorOutlinerTableModel::NodeRemoved(cs::SGNode *node)
{
  endRemoveRows();
}


QModelIndex ShaderGraphEditorOutlinerTableModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid() || !m_shaderGraph)
  {
    return QModelIndex();
  }
  return createIndex(row, column, (void*)this);
}

QModelIndex ShaderGraphEditorOutlinerTableModel::parent(const QModelIndex &child) const
{
  return QModelIndex();
}

int ShaderGraphEditorOutlinerTableModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid() || !m_shaderGraph)
  {
    return 0;
  }
  return m_shaderGraph->GetNumberOfTotalNodes() + 1;
}

int ShaderGraphEditorOutlinerTableModel::columnCount(const QModelIndex &parent) const
{
  return 2;
}

QVariant ShaderGraphEditorOutlinerTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  if (role == Qt::DisplayRole)
  {
    if (index.row() == 0)
    {
      switch (index.column())
      { 
      case 0:
        return tr("ShaderGraph");
      default:
        return QVariant();
      }
    }
    else 
    {
      cs::SGNode *node = m_shaderGraph->GetNode(index.row() - 1);
      cs::SGResourceNode *resourceNode = cs::QueryClass<cs::SGResourceNode>(node);
      const cs::Class *cls = node->GetClass();
      switch (index.column())
      {
      case 0:
        return QString(cls->GetName().c_str());
      case 1:
        if (resourceNode)
        {
          return QString(resourceNode->GetResourceName().c_str());
        }
        return QVariant();
      }
    }
  }
  return QVariant();
}

QVariant ShaderGraphEditorOutlinerTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
  {
    return QVariant();
  }

  switch (section)
  {
  case 0:
    return tr("Type");
  case 1:
    return tr("Resource");
  }
  return QVariant();
}


QModelIndex ShaderGraphEditorOutlinerTableModel::GetIndex(cs::SGShaderGraph *graph, int column)
{
  return createIndex(0, column, (void*)this);
}

QModelIndex ShaderGraphEditorOutlinerTableModel::GetIndex(cs::SGNode *node, int column)
{
  for (csSize i = 0, in = m_shaderGraph->GetNumberOfTotalNodes(); i < in; ++i)
  {
    if (m_shaderGraph->GetNode(i) == node)
    {
      return createIndex(i + 1, column, (void*)this);
    }
  }
  return QModelIndex();
}


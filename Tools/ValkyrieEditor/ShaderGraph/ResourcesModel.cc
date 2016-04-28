

#include <ShaderGraph/ResourcesModel.hh>


namespace shadergraph
{


ResourcesModel::ResourcesModel()
  : QAbstractItemModel()
{

}

ResourcesModel::~ResourcesModel()
{
  m_nodes.clear();
}


void ResourcesModel::AddNode(SGNode *node)
{
  if (m_nodes.contains(node))
  {
    return;
  }


  int row = m_nodes.size();
  beginInsertRows(QModelIndex(), row, row);
  m_nodes.append(node);
  endInsertRows();
}

void ResourcesModel::RemoveNode(SGNode *node)
{
  int row = m_nodes.indexOf(node);
  if (row == -1)
  {
    return;
  }
  
  beginRemoveRows(QModelIndex(), row, row);
  m_nodes.removeAt(row);
  endInsertRows();
}

void ResourcesModel::NodeChanged(SGNode *node)
{
  int row = m_nodes.indexOf(node);
  if (row == -1)
  {
    return;
  }

  emit dataChanged(createIndex(row, 0, node), createIndex(row, 2, node));
}

QModelIndex ResourcesModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid() || row < 0 || row >= m_nodes.size())
  {
    return QModelIndex();
  }

  return createIndex(row, column, m_nodes[row]);
}


QModelIndex ResourcesModel::parent(const QModelIndex &child) const
{
  return QModelIndex();
}


int ResourcesModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return 0;
  }

  return  m_nodes.size();
}


int ResourcesModel::columnCount(const QModelIndex &parent) const
{
  return 3;
}


static QString type_name_map[] = {
  QString("Float"),
  QString("Vec2"),
  QString("Vec3"),
  QString("Vec4"),
  QString("Int"),
  QString("IVec2"),
  QString("IVec3"),
  QString("IVec4"),
  QString("Color4"),
  QString("Mat3"),
  QString("Mat4"),
  QString("Texture"),
};

QVariant ResourcesModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  SGNode *node = static_cast<SGNode*>(index.internalPointer());
  if (role == Qt::DisplayRole)
  {
    switch (index.column())
    {
    case 0:
      return node->GetName();
    case 1:
      return type_name_map[node->GetResourceType()];
    case 2:
      return GetValue(node);
    default:
      break;
    }
  }
  return QVariant();
}

QVariant ResourcesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole)
  {
    switch (section)
    {
    case 0:
      return tr("Name");
    case 1:
      return tr("Type");
    case 2:
      return tr("Default");
    }
  }
  return QVariant();
}


QString ResourcesModel::GetValue(SGNode *node) const
{
  float *floats = node->GetDefaultFloat();
  int *ints = node->GetDefaultInt();
  vkResourceLocator &resource = node->GetDefaultTexture();
  switch (node->GetResourceType())
  {
  case eSPT_Float:
    return QString("%1").arg(floats[0]);
  case eSPT_Vector2:
    return QString("%1, %2").arg(floats[0]).arg(floats[1]);
  case eSPT_Vector3:
    return QString("%1, %2, %3").arg(floats[0]).arg(floats[1]).arg(floats[2]);
  case eSPT_Vector4:
    return QString("%1, %2, %3, %4").arg(floats[0]).arg(floats[1]).arg(floats[2]).arg(floats[3]);

  case eSPT_Int:
    return QString("%1").arg(ints[0]);
  case eSPT_IVector2:
    return QString("%1, %2").arg(ints[0]).arg(ints[1]);
  case eSPT_IVector3:
    return QString("%1, %2, %3").arg(ints[0]).arg(ints[1]).arg(ints[2]);
  case eSPT_IVector4:
    return QString("%1, %2, %3, %4").arg(ints[0]).arg(ints[1]).arg(ints[2]).arg(ints[3]);

  case eSPT_Color4:
    return QString("%1, %2, %3, %4").arg(floats[0]).arg(floats[1]).arg(floats[2]).arg(floats[3]);

  case eSPT_Matrix3:
    return QString("<multiple values>");

  case eSPT_Matrix4:
    return QString("<multiple values>");

  case eSPT_Texture:
    return QString(resource.GetResourceFile().c_str());
  }
  return QString("");
}

}

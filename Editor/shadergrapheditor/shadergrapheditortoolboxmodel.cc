

#include <shadergrapheditor/shadergrapheditortoolboxmodel.hh>
#include <cobalt/graphics/shadergraph/cssgnodes.hh>
#include <QMimeData>
#include <QDataStream>


ShaderGraphEditorToolboxModel::ShaderGraphEditorToolboxModel()
  : QAbstractItemModel()
  , m_root(0)
{

  CreateModelData("");
}

void ShaderGraphEditorToolboxModel::CreateModelData(const QString &filter)
{
  beginResetModel();
  if (m_root)
  {
    delete m_root;
  }

  m_root = new Data("root");
  cs::SGNodes *nodes = cs::SGNodes::Get();
  for (const cs::SGNodes::Entry &entry : nodes->GetEntries())
  {
    QString v(entry.name.c_str());
    if (filter.isEmpty() || v.contains(filter, Qt::CaseInsensitive))
    {
      Data *data = fromString(entry.name);
      data->cls = entry.clazz;
    }
  }
  endResetModel();
}

ShaderGraphEditorToolboxModel::Data *ShaderGraphEditorToolboxModel::fromString(const std::string &string)
{
  Data *d = m_root;
  std::string name = string;
  while (true)
  {
    std::string dataName = name;

    size_t idx = name.find("/");
    if (idx != -1)
    {
      dataName = name.substr(0, idx);
    }

    bool found = false;
    for (Data *child : d->children)
    {
      if (child->name == dataName)
      {
        d = child;
        found = true;
      }
    }
    if (!found)
    {
      Data *nd = new Data(dataName);
      nd->parent = d;
      d->children.push_back(nd);
      d = d->children[d->children.size() - 1];
    }
    if (idx == -1)
    {
      break;
    }
    else
    {
      name = name.substr(idx + 1);
    }
  }
  return d;
}

ShaderGraphEditorToolboxModel::~ShaderGraphEditorToolboxModel()
{

}

QModelIndex ShaderGraphEditorToolboxModel::index(int row, int column, const QModelIndex &parent) const
{
  Data *data = reinterpret_cast<Data*>(parent.internalPointer());
  if (data)
  {
    return createIndex(row, column, data->children[row]);
  }
  return createIndex(row, column, m_root->children[row]);
}

QModelIndex ShaderGraphEditorToolboxModel::parent(const QModelIndex &child) const
{
  Data *data = reinterpret_cast<Data*>(child.internalPointer());
  Data *parent = data->parent;
  if (!parent || parent == m_root)
  {
    return QModelIndex();
  }


  int idx = 0;
  if (parent->parent)
  {
    idx = parent->parent->children.indexOf(parent);
  }
  return createIndex(idx, 0, parent);
}

int ShaderGraphEditorToolboxModel::rowCount(const QModelIndex &parent) const
{
  Data *data = reinterpret_cast<Data*>(parent.internalPointer());
  if (data)
  {
    return data->children.size();
  }
  return m_root->children.size();
}

int ShaderGraphEditorToolboxModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant ShaderGraphEditorToolboxModel::data(const QModelIndex &index, int role) const
{
  Data *data = reinterpret_cast<Data*>(index.internalPointer());
  if (!data || index.column() != 0)
  {
    return QVariant();
  }

  switch (role)
  {
  case Qt::DisplayRole:
    return QString(data->name.c_str());
  default:
    break;
  }
  return QVariant();
}

Qt::ItemFlags ShaderGraphEditorToolboxModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags flags = QAbstractItemModel::flags(index);
  Data *data = reinterpret_cast<Data*>(index.internalPointer());
  if (data && data->cls)
  {
    flags |= Qt::ItemIsDragEnabled;
  }
  return flags;
}

QStringList ShaderGraphEditorToolboxModel::mimeTypes() const
{
  QStringList types;
  types << SHADER_GRAPH_EDITO_TOOLBOX_MODEL_CLASS_MIME;
  return types;
}

QMimeData *ShaderGraphEditorToolboxModel::mimeData(const QModelIndexList &indexes) const
{
  QMimeData *mimeData = new QMimeData();
  QByteArray encodedData;

  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  for(const QModelIndex &index : indexes) 
  {
    Data *data = reinterpret_cast<Data*>(index.internalPointer());
    if (data && data->cls)
    {
      stream << QString(data->cls->GetName().c_str());
    }
  }

  mimeData->setData(SHADER_GRAPH_EDITO_TOOLBOX_MODEL_CLASS_MIME, encodedData);
  return mimeData;
}


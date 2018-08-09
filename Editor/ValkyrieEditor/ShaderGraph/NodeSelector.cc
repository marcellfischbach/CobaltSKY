
#include <ShaderGraph/NodeSelector.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>

#include <qlayout.h>
#include <qtimer.h>
#include <qvector.h>



class NodesModel : public QAbstractItemModel
{
public:
  NodesModel();
  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QModelIndex SetFilter(const QString &filter);
  const vkClass *GetClass (const QModelIndex &index) const;

private:
  struct Node
  {
    QVector<Node*> m_children;
    Node *m_parent;
    const vkClass *m_class;
    QString m_name;

  };
  Node *m_root;

  Node *FindNode(const QString &category);
  void ClearNode(Node *node);
};

NodeSelector::NodeSelector(QWidget *parent)
  : QDialog(parent)
{
  m_gui.setupUi(this);
  setModal(true);

  m_nodesModel = new NodesModel();
  m_gui.tvNodes->setModel(m_nodesModel);

  /*

  m_listView = new QListView(this);

  QGridLayout *layout = new QGridLayout(this);
  layout->setMargin(0);
  layout->addWidget(m_listView, 0, 0, 1, 1);
 

  connect (m_listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_listView_doubleClicked(QModelIndex)));
  */
}

void NodeSelector::on_tvNodes_doubleClicked(const QModelIndex &index)
{
  //selectIndex(index);
}

void NodeSelector::on_tvNodes_activated(const QModelIndex &index)
{
  selectIndex(index);
}

void NodeSelector::selectIndex(const QModelIndex &index)
{
  if (!index.isValid())
  {
    return;
  }

  const vkClass *clazz = m_nodesModel->GetClass(index);
  if (!clazz)
  {
    return;
  }

  emit addNode(clazz);

  close();
}


void NodeSelector::on_leFilter_textChanged(const QString &text)
{
  QModelIndex index = m_nodesModel->SetFilter(text);
  if (index.isValid())
  {
    m_gui.tvNodes->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
    m_gui.tvNodes->expandAll();
  }
}


NodesModel::NodesModel()
  : QAbstractItemModel()
  , m_root(0)
{
  SetFilter("");
}

QModelIndex NodesModel::SetFilter(const QString &filter)
{
  beginResetModel();
  ClearNode(m_root);
  m_root = new Node();
  Node *firstNode = 0;
  int nodeCount;
  const std::vector<vkSGNodes::Entry > entries = vkSGNodes::Get()->GetEntries();
  for (const vkSGNodes::Entry &entry : entries)
  {
    QString entryName(entry.name.c_str());
    if (filter.length() == 0 || entryName.contains(filter, Qt::CaseInsensitive))
    {
      Node *node = FindNode(entryName);
      node->m_class = entry.clazz;
      if (!firstNode)
      {
        firstNode = node;
      }
    }
  }

  endResetModel();
  if (!firstNode)
  {
    return QModelIndex();
  }

  return createIndex(firstNode->m_parent->m_children.indexOf(firstNode), 0, firstNode);
}

void NodesModel::ClearNode(Node *node)
{
  if (!node)
  {
    return;
  }
  for (Node *n : node->m_children)
  {
    ClearNode(n);
  }
  node->m_children.clear();
  delete node;
}

NodesModel::Node *NodesModel::FindNode(const QString &fullName)
{
  QString name = fullName;
  QStringList parts = name.split("/");
  Node *node = m_root;
  while (parts.size() > 1)
  {
    QString firstPart = parts[0];
    parts.pop_front();

    bool needNewNode = true;
    for (Node *n : node->m_children)
    {
      if (n->m_name == firstPart)
      {
        node = n;
        needNewNode = false;
        break;
      }
    }

    if (needNewNode)
    {
      Node *n = new Node();
      n->m_name = firstPart;
      n->m_parent = node;
      node->m_children.push_back(n);
      node = n;
    }
  }

  if (parts.size() == 1)
  {
    Node *n = new Node();
    n->m_name = parts[0];
    n->m_parent = node;
    node->m_children.push_back(n);
    return n;
  }
  return 0;
}


const vkClass *NodesModel::GetClass(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return 0;
  }

  const Node *node = static_cast<const Node*>(index.internalPointer());
  if (node)
  {
    return node->m_class;
  }
  return 0;
}

QModelIndex NodesModel::index(int row, int column, const QModelIndex &parent) const
{
  Node *node = m_root;
  if (parent.isValid())
  {
    node = static_cast<Node*>(parent.internalPointer());
  }
  return createIndex(row, column, node->m_children[row]);
}

QModelIndex NodesModel::parent(const QModelIndex &child) const
{
  Node *node = static_cast<Node*>(child.internalPointer());
  Node *parent = node->m_parent;
  if (parent == m_root)
  {
    return QModelIndex();
  }
  Node *parentParent = parent->m_parent;
  return createIndex(parentParent->m_children.indexOf(parent), child.column(), parent);
}

int NodesModel::rowCount(const QModelIndex &parent) const
{
  Node *node = m_root;
  if (parent.isValid())
  {
    node = static_cast<Node*>(parent.internalPointer());
  }

  return node->m_children.size();
}

int NodesModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant NodesModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  Node *node = static_cast<Node*>(index.internalPointer());
  if (role == Qt::DisplayRole)
  {
    return node->m_name;
  }
  else if (role == Qt::BackgroundRole)
  {
    if (node->m_parent == m_root)
    {
      return QVariant(QColor(0, 0, 127));
    }
  }
  else if (role == Qt::ForegroundRole)
  {
    if (node->m_parent == m_root)
    {
      return QVariant(QColor(255, 255, 255));
    }
  }
  return QVariant ();
}

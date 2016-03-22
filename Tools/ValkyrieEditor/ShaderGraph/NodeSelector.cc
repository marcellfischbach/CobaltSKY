
#include <ShaderGraph/NodeSelector.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>

#include <qlayout.h>
#include <qtimer.h>



class NodesModel : public QAbstractItemModel
{
public:
  NodesModel();
  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  const vkClass *GetClass (int row) const;

private:
  std::vector<vkSGNodes::Entry> m_entries;

};

NodeSelector::NodeSelector(QWidget *parent)
  : QDialog(parent)
{
  setModal(true);


  m_listView = new QListView(this);

  QGridLayout *layout = new QGridLayout(this);
  layout->setMargin(0);
  layout->addWidget(m_listView, 0, 0, 1, 1);

  m_nodesModel = new NodesModel();
  m_listView->setModel(m_nodesModel);

  connect (m_listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_listView_doubleClicked(QModelIndex)));
}

void NodeSelector::on_listView_doubleClicked(const QModelIndex &index)
{
  if (!index.isValid())
  {
    return;
  }

  const vkClass *clazz = m_nodesModel->GetClass(index.row());
  if (!clazz)
  {
    return;
  }

  emit addNode(clazz);

  close();
}


NodesModel::NodesModel()
  : QAbstractItemModel()
{
  m_entries = vkSGNodes::Get()->GetEntries();
}

const vkClass *NodesModel::GetClass(int row) const
{
  if (row < 0 || row >= m_entries.size())
  {
    return 0;
  }

  const vkSGNodes::Entry &entry = m_entries[row];
  return entry.clazz;
}

QModelIndex NodesModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return QModelIndex();
  }

  return createIndex(row, column);
}

QModelIndex NodesModel::parent(const QModelIndex &child) const
{
  return QModelIndex();
}

int NodesModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return 0;
  }
  return (int)m_entries.size();
}

int NodesModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant NodesModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole)
  {
    const vkSGNodes::Entry &entry = m_entries[index.row()];
    return QString(entry.name.c_str());
  }
  return QVariant ();
}

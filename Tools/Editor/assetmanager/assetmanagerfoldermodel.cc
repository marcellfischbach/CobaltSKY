
#include <assetmanager/assetmanagerfoldermodel.hh>
#include <editor.hh>
#include <QDir>
#include <QList>

class AssetManagerFolderModelEntry
{
public:
  AssetManagerFolderModelEntry(const QDir &dir, AssetManagerFolderModelEntry *parent = 0)
    : m_dir (dir)
    , m_parent (parent)
    , m_scanned(false)
  {
    if (!m_parent)
    {
      m_name = QString("(%1)").arg(QObject::tr("root"));
    }
    else
    {
      m_name = m_dir.dirName();
    }
  }

  AssetManagerFolderModelEntry::~AssetManagerFolderModelEntry()
  {
    for (AssetManagerFolderModelEntry *entry : m_entries)
    {
      delete entry;
    }
    m_entries.clear();
  }

  const QString &GetName() const
  {
    return m_name;
  }

  const QDir &GetDir()
  {
    return m_dir;
  }

  const QList<AssetManagerFolderModelEntry*> GetEntries()
  {
    Scan();
    return m_entries;
  }

  AssetManagerFolderModelEntry *GetParent()
  {
    return m_parent;
  }

 
  int Index()
  {
    if (m_parent)
    {
      return m_parent->m_entries.indexOf(this);
    }
    return 0;
  }

private:
  void Scan()
  {
    if (m_scanned)
    {
      return;
    }

    m_entries.clear();
    QStringList subFolders = m_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Name);
    for (QString subFolder : subFolders)
    {
      QDir dir(m_dir);
      dir.cd(subFolder);
      m_entries.push_back(new AssetManagerFolderModelEntry(dir, this));
    }
    m_scanned = true;
  }


  QDir m_dir;
  QString m_name;

  AssetManagerFolderModelEntry* m_parent;
  QList<AssetManagerFolderModelEntry*> m_entries;
  bool m_scanned;
};

#define FROM_INDEX(e) reinterpret_cast<AssetManagerFolderModelEntry*>(e.internalPointer());


AssetManagerFolderModel::AssetManagerFolderModel()
  : QAbstractItemModel()
{
  m_root = new AssetManagerFolderModelEntry(Editor::Get()->GetRootPath());
}

AssetManagerFolderModel::~AssetManagerFolderModel()
{

}


QModelIndex AssetManagerFolderModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!parent.isValid() && row == 0)
  {
    return createIndex(0, column, m_root);
  }
  else if (parent.isValid())
  {
    AssetManagerFolderModelEntry *parentEntry = FROM_INDEX(parent);
    if (row < parentEntry->GetEntries().size())
    {
      return createIndex(row, column, parentEntry->GetEntries()[row]);
    }
  }
  return QModelIndex();
}

QModelIndex AssetManagerFolderModel::parent(const QModelIndex &child) const
{
  if (!child.isValid())
  {
    return QModelIndex();
  }
  AssetManagerFolderModelEntry* childEntry = FROM_INDEX(child);
  AssetManagerFolderModelEntry* parent = childEntry->GetParent();
  if (!parent)
  {
    return QModelIndex();
  }

  return createIndex(parent->Index(), child.column(), parent);
}

int AssetManagerFolderModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
  {
    return 1;
  }
  AssetManagerFolderModelEntry *parentEntry = FROM_INDEX(parent);
  return parentEntry->GetEntries().size();
}

int AssetManagerFolderModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant AssetManagerFolderModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole && index.isValid())
  {
    AssetManagerFolderModelEntry *entry = FROM_INDEX(index);
    return QVariant(entry->GetName());
  }
  return QVariant();
}

QDir AssetManagerFolderModel::GetDir(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return Editor::Get()->GetRootPath();
  }
  AssetManagerFolderModelEntry *entry = FROM_INDEX(index);
  return entry->GetDir();
}

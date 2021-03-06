
#include <editor/assetmanager/assetmanagerfoldermodel.hh>
#include <editor/editor.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csvfs.hh>
#include <QDir>
#include <QList>

class AssetManagerFolderModelEntry
{
public:
  enum Type
  {
    Root,
    Entry,
    Dir,
  };

  AssetManagerFolderModelEntry()
    : m_dir(QDir())
    , m_parent(0)
    , m_scanned(true)
    , m_type(Root)
  {
    m_name = QObject::tr("AssetManager");
  }

  AssetManagerFolderModelEntry(const cs::VFS::Entry &entry)
    : m_parent(0)
    , m_scanned(false)
    , m_type(Entry)
  {
    m_dir = QDir(entry.GetAbsPath().c_str());
    m_name = QString(entry.GetName().c_str());
  }

  AssetManagerFolderModelEntry(const QDir &dir)
    : m_dir(dir)
    , m_parent(0)
    , m_scanned(false)
    , m_type(Dir)
  {
    m_name = m_dir.dirName();
  }

  ~AssetManagerFolderModelEntry()
  {
    for (AssetManagerFolderModelEntry *entry : m_entries)
    {
      delete entry;
    }
    m_entries.clear();
  }

  void AddEntry(AssetManagerFolderModelEntry *entry)
  {
    m_entries.push_back(entry);
    entry->m_parent = this;
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

  cs::ResourceLocator GetResourceLocator() const
  {
    switch (m_type)
    {
    case Root:
      return cs::ResourceLocator();
    case Entry:
      return cs::ResourceLocator(cs::ResourceEntry(std::string((const char*)m_name.toLatin1())));
    case Dir:
      if (!m_parent)
      {
        return cs::ResourceLocator();
      }
      else
      {
        cs::ResourceLocator parentLocator = m_parent->GetResourceLocator();
		return cs::ResourceLocator(
			cs::ResourceEntry(parentLocator.GetResourceEntry()),
			cs::ResourceFile(parentLocator.GetResourceFile() + "/" + std::string((const char*)m_name.toLatin1())));
      }
    }
    return cs::ResourceLocator();
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
      AddEntry(new AssetManagerFolderModelEntry(dir));
    }
    m_scanned = true;
  }


  Type m_type;
  QDir m_dir;
  QString m_name;
  QString m_entryName;

  AssetManagerFolderModelEntry* m_parent;
  QList<AssetManagerFolderModelEntry*> m_entries;
  bool m_scanned;
};

#define FROM_INDEX(e) reinterpret_cast<AssetManagerFolderModelEntry*>(e.internalPointer());


AssetManagerFolderModel::AssetManagerFolderModel()
  : QAbstractItemModel()
{
  m_root = new AssetManagerFolderModelEntry();
  for (csSize i = 0, in = cs::VFS::Get()->GetNumberOfEntries(); i < in; ++i)
  {
    const cs::VFS::Entry &entry = cs::VFS::Get()->GetEntry(i);
    m_root->AddEntry(new AssetManagerFolderModelEntry(entry));
  }
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

cs::ResourceLocator AssetManagerFolderModel::GetResourceLocator(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return cs::ResourceLocator();
  }
  AssetManagerFolderModelEntry *entry = FROM_INDEX(index);
  return entry->GetResourceLocator();
}

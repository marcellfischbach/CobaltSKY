

#include <assetmanager/assetmanagercontentmodel.hh>
#include <assetmanager/assetmanagercontentmodelentry.hh>
#include <editor.hh>
#include <editorimage.hh>
#include <QPixmap>
#include <QMimeData>
#include <mimehelper.hh>
#include <valkyrie/core/ifile.hh>
#include <valkyrie/core/vkfileinfo.hh>
#include <valkyrie/core/vkvfs.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <QDomDocument>
#include <QFile>

#define FROM_INDEX(e) reinterpret_cast<AssetManagerContentModelEntry*>(e.internalPointer());
#define CONST_FROM_INDEX(e) reinterpret_cast<const AssetManagerContentModelEntry*>(e.internalPointer());

AssetManagerContentModel::AssetManagerContentModel()
  : QAbstractItemModel()
{
}

AssetManagerContentModel::~AssetManagerContentModel()
{
  CleanupEntries();
}


void AssetManagerContentModel::SetResourceLocator(const vkResourceLocator &locator)
{
  if (m_locator == locator)
  {
    return;
  }


  m_locator = locator;
  vkString absFileName = "";
  if (!locator.GetResourceEntry().empty())
  {
    absFileName = vkVFS::Get()->GetAbsolutePath(locator.GetResourceFile(), locator.GetResourceEntry());
  }
  else
  {
    absFileName = vkVFS::Get()->GetAbsolutePath(locator.GetResourceFile());
  }

  beginResetModel();
  CleanupEntries();
  QDir dir (QString(absFileName.c_str()));
  QStringList nameFilters;
  nameFilters.append("*.xasset");
  QStringList fileNames = dir.entryList(nameFilters, QDir::Files, QDir::Name);
  for (const QString &fileName : fileNames)
  {
    vkString resourceFileName = locator.GetResourceFile() + "/" + vkString((const char*)fileName.toLatin1());
    vkResourceLocator fileLocator(resourceFileName, "", locator.GetResourceEntry());
    m_entries.push_back(new AssetManagerContentModelEntry(fileLocator));
  }

  endResetModel();
}

void AssetManagerContentModel::CleanupEntries()
{
  for (AssetManagerContentModelEntry *entry : m_entries)
  {
    delete entry;
  }
  m_entries.clear();
}

QModelIndex AssetManagerContentModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid() || column != 0 || row >= m_entries.size())
  {
    return QModelIndex();
  }

  return createIndex(row, 0, m_entries[row]);
}

QModelIndex AssetManagerContentModel::parent(const QModelIndex &child) const
{
  return QModelIndex();
}

int AssetManagerContentModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    return 0;
  }
  int num = m_entries.size();
  return num;
}

int AssetManagerContentModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant AssetManagerContentModel::data(const QModelIndex &index, int role) const
{
  if (index.isValid())
  {
    AssetManagerContentModelEntry *entry = FROM_INDEX(index);
    switch (role)
    {
    case Qt::DisplayRole:
      return QVariant(entry->GetEntryName());
    case Qt::DecorationRole:
      return QVariant(entry->GetIcon());
    }
  }
  return QVariant();
}

QStringList AssetManagerContentModel::mimeTypes() const
{
  printf("mimeTypes\n");
  QStringList result;
  result << QString(MimeHelper::GetResourceLocatorMimeType().c_str());
  result << QString(MimeHelper::GetResourceTypeMimeType().c_str());
  result << QString(MimeHelper::GetClassMimeType().c_str());
  return result;
}

QMimeData *AssetManagerContentModel::mimeData(const QModelIndexList &indexes) const
{
  printf("mimeData: %d\n", indexes.size());
  if (indexes.size() != 1)
  {
    return 0;
  }
  const QModelIndex &index = indexes[0];
  const AssetManagerContentModelEntry *entry = CONST_FROM_INDEX(index);

  const vkResourceLocator &locator = entry->GetLocator();
  const vkClass *cls = vkResourceManager::Get()->EvalClass(locator);

  QMimeData *data = new QMimeData();
  MimeHelper::PutResourceLocatorMimeData(data, locator.GetResourceFile());
  MimeHelper::PutResourceTypeMimeData(data, ReadType(locator));
  MimeHelper::PutClassMimeData(data, cls);
  return data;
}

Qt::ItemFlags AssetManagerContentModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

  if (index.isValid())
    return Qt::ItemIsDragEnabled | defaultFlags;
  else
    return defaultFlags;
}

vkResourceLocator AssetManagerContentModel::GetLocator(const QModelIndex &index) const
{
  const AssetManagerContentModelEntry *entry = CONST_FROM_INDEX(index);
  if (!entry)
  {
    return vkResourceLocator();
  }
  return entry->GetLocator();
}

const AssetManagerContentModelEntry *AssetManagerContentModel::GetEntry(const QModelIndex &index) const
{
  return CONST_FROM_INDEX(index);
}

vkString AssetManagerContentModel::ReadType(const vkResourceLocator &locator) const
{
  vkString path = vkVFS::Get()->GetAbsolutePath(locator.GetResourceFile(), locator.GetResourceEntry());
  QFile file(QString(path.c_str()));
  file.open(QIODevice::ReadOnly);
  QDomDocument doc;
  QString errorMesage;
  int line, column;
  if (!doc.setContent(&file, &errorMesage, &line, &column))
  {
    printf("unable to parse %s '%s' %d:%d\n", path.c_str(), (const char*)errorMesage.toLatin1(), line, column);
    return "";
  }

  QDomElement assetElement = doc.documentElement();
  if (assetElement.isNull () || assetElement.tagName() != QString("asset"))
  {
    return "";
  }

  QDomElement dataElement = assetElement.firstChildElement("data");
  if (dataElement.isNull())
  {
    return "";
  }

  QDomElement typeElement = dataElement.firstChildElement();
  if (typeElement.isNull())
  {
    return "";
  }

  return vkString((const char*)typeElement.tagName().toLatin1());
}



#include <assetmanager/assetmanagercontentmodel.hh>
#include <editor.hh>
#include <editorimage.hh>
#include <QPixmap>
#include <QMimeData>
#include <mimehelper.hh>
#include <valkyrie/core/ifile.hh>
#include <valkyrie/core/vkvfs.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <QDomDocument>
#include <QFile>

class AssetManagerContentModelEntry
{
public:
  AssetManagerContentModelEntry(const QDir &dir, const QString &fileName)
    : m_dir(dir)
    , m_entryName(fileName)
  {
    QString fn = m_dir.absoluteFilePath(m_entryName);
    m_fileName = (const char*)fn.toLatin1();
    vkString res = Editor::Get()->ConvertToResourcePath(m_fileName);
    vkResourceLocator previewLocator(res, "preview");
    EditorImage *editorImage = vkResourceManager::Get()->Aquire<EditorImage>(previewLocator);
    if (editorImage)
    {
      QImage img = editorImage->GetImage();
      if (!img.isNull())
      {
        m_pixmap = QPixmap::fromImage(img);
      }
    }
    if (m_pixmap.isNull())
    {
      m_pixmap = QPixmap(":/icons/resources/UnknownAsset64.png");
    }
  }

  vkString GetFileName() const
  {
    return  m_fileName;

  }

  const QPixmap &GetIcon() const
  {
    return m_pixmap;
  }

  const QString &GetEntryName() const
  {
    return m_entryName;
  }


private:
  QDir m_dir;
  QString m_entryName;
  QPixmap m_pixmap;
  vkString m_fileName;
};

#define FROM_INDEX(e) reinterpret_cast<AssetManagerContentModelEntry*>(e.internalPointer());
#define CONST_FROM_INDEX(e) reinterpret_cast<const AssetManagerContentModelEntry*>(e.internalPointer());

AssetManagerContentModel::AssetManagerContentModel()
  : QAbstractItemModel()
{
  SetDir(QDir());
}

AssetManagerContentModel::~AssetManagerContentModel()
{
  CleanupEntries();
}

void AssetManagerContentModel::SetDir(const QDir &dir)
{
  if (dir == m_dir)
  {
    return;
  }
  beginResetModel();
  CleanupEntries();

  m_dir = dir;
  QStringList nameFilters;
  nameFilters.append("*.xasset");
  QStringList fileNames = dir.entryList(nameFilters, QDir::Files, QDir::Name);
  for (const QString &fileName : fileNames)
  {
    m_entries.push_back(new AssetManagerContentModelEntry(m_dir, fileName));
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

  QMimeData *data = new QMimeData();
  MimeHelper::PutResourceLocatorMimeData(data, ExtractResourceName(entry->GetFileName()));
  MimeHelper::PutResourceTypeMimeData(data, ReadType(entry->GetFileName()));
  MimeHelper::PutClassMimeData(data, ReadClass(entry->GetFileName()));
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

vkString AssetManagerContentModel::GetEntry(const QModelIndex &index) const
{
  const AssetManagerContentModelEntry *entry = CONST_FROM_INDEX(index);
  if (!entry)
  {
    return "";
  }
  return entry->GetFileName();
}

vkString AssetManagerContentModel::ExtractResourceName(const vkString &fileName) const
{
  return Editor::Get()->ConvertToResourcePath(fileName);
}

vkString AssetManagerContentModel::ReadType(const vkString &fileName) const
{

  QFile file(QString(fileName.c_str()));
  file.open(QIODevice::ReadOnly);
  QDomDocument doc;
  QString errorMesage;
  int line, column;
  if (!doc.setContent(&file, &errorMesage, &line, &column))
  {
    printf("unable to parse %s '%s' %d:%d\n", fileName.c_str(), (const char*)errorMesage.toLatin1(), line, column);
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

const vkClass *AssetManagerContentModel::ReadClass(const vkString &fileName) const
{
  vkString resourceName = Editor::Get()->ConvertToResourcePath(fileName);
  return vkResourceManager::Get()->EvalClass(vkResourceLocator(resourceName));
}

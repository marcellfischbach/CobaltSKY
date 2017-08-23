

#include <assetmanager/assetmanagercontentmodel.hh>
#include <assetmanager/assetmanagercontentmodelentry.hh>
#include <editor.hh>
#include <editorimage.hh>
#include <eventbus.hh>
#include <events/assetpreviewiconchangedevent.hh>
#include <QPixmap>
#include <QMimeData>
#include <mimehelper.hh>
#include <cobalt/core/ifile.hh>
#include <cobalt/core/csevent.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <QDomDocument>
#include <QFile>

#define FROM_INDEX(e) reinterpret_cast<AssetManagerContentModelEntry*>(e.internalPointer());
#define CONST_FROM_INDEX(e) reinterpret_cast<const AssetManagerContentModelEntry*>(e.internalPointer());


void asset_manager_content_mode_preview_icon_changed(csEvent &event, void *user)
{
  AssetPreviewIconChangedEvent &evt = static_cast<AssetPreviewIconChangedEvent&>(event);
  AssetManagerContentModel *model = reinterpret_cast<AssetManagerContentModel*>(user);
  if (model)
  {
    model->PreviewIconChanged(evt.GetLocator());
  }
}

AssetManagerContentModel::AssetManagerContentModel()
  : QAbstractItemModel()
{
  EventBus::Get().Register(AssetPreviewIconChangedEvent::GetStaticClass(), asset_manager_content_mode_preview_icon_changed, this);
}

AssetManagerContentModel::~AssetManagerContentModel()
{
  EventBus::Get().Deregister(asset_manager_content_mode_preview_icon_changed, this);
  CleanupEntries();
}


void AssetManagerContentModel::SetResourceLocator(const csResourceLocator &locator)
{
  if (m_locator == locator)
  {
    return;
  }


  m_locator = locator;

  Refresh();
}

const csResourceLocator &AssetManagerContentModel::GetResourceLocator() const
{
  return m_locator;
}

void AssetManagerContentModel::Refresh()
{
  std::string absFileName = "";
  if (!m_locator.GetResourceEntry().empty())
  {
    absFileName = csVFS::Get()->GetAbsolutePath(m_locator.GetResourceFile(), m_locator.GetResourceEntry());
  }
  else
  {
    absFileName = csVFS::Get()->GetAbsolutePath(m_locator.GetResourceFile());
  }

  printf("Reset: %s\n", m_locator.GetResourceFile().c_str());

  beginResetModel();
  CleanupEntries();
  QDir dir (QString(absFileName.c_str()));
  QStringList nameFilters;
  nameFilters.append("*.xasset");
  QStringList fileNames = dir.entryList(nameFilters, QDir::Files, QDir::Name);
  for (const QString &fileName : fileNames)
  {
    std::string resourceFileName = m_locator.GetResourceFile() + "/" + std::string((const char*)fileName.toLatin1());
    printf("  %s\n", resourceFileName.c_str());
    csResourceLocator fileLocator(resourceFileName, "", m_locator.GetResourceEntry());
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

  const csResourceLocator &locator = entry->GetLocator();
  const csClass *cls = csResourceManager::Get()->EvalClass(locator);

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

csResourceLocator AssetManagerContentModel::GetLocator(const QModelIndex &index) const
{
  const AssetManagerContentModelEntry *entry = CONST_FROM_INDEX(index);
  if (!entry)
  {
    return csResourceLocator();
  }
  return entry->GetLocator();
}

const AssetManagerContentModelEntry *AssetManagerContentModel::GetEntry(const QModelIndex &index) const
{
  return CONST_FROM_INDEX(index);
}

std::string AssetManagerContentModel::ReadType(const csResourceLocator &locator) const
{
  std::string path = csVFS::Get()->GetAbsolutePath(locator.GetResourceFile(), locator.GetResourceEntry());
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

  return std::string((const char*)typeElement.tagName().toLatin1());
}


void AssetManagerContentModel::PreviewIconChanged(const csResourceLocator &locator)
{
  for (AssetManagerContentModelEntry *entry : m_entries)
  {
    if (entry->GetLocator() == locator)
    {
      entry->ReloadIcon();
      int idx = m_entries.indexOf(entry);
      QModelIndex index = createIndex(idx, 0, entry);
      emit dataChanged(index, index);
    }
  }
}
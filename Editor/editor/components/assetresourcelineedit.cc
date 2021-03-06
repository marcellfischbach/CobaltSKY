
#include <editor/components/assetresourcelineedit.hh>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <editor/mimehelper.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <editor/assetmodel/asset.hh>

AssetResourceLineEdit::AssetResourceLineEdit(QWidget *parent)
  : QLineEdit(parent)
{
  printf("AssetResourceLineEdit::AssetResourceLineEdit 1\n");
}

AssetResourceLineEdit::AssetResourceLineEdit(const QString &contents, QWidget *parent)
  : QLineEdit(contents, parent)
{
  printf("AssetResourceLineEdit::AssetResourceLineEdit 2\n");
}

AssetResourceLineEdit::~AssetResourceLineEdit()
{

}

void AssetResourceLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
  const QMimeData *mimeData = event->mimeData();
  if (!mimeData->hasFormat("application/assetModelEntryPtr"))
  {
    return;
  }
  QByteArray rawData = mimeData->data("application/assetModelEntryPtr");
  QDataStream entriesStream(&rawData, QIODevice::ReadOnly);
  std::vector<asset::model::Entry*> entries;
  get(entriesStream, entries);
  if (entries.size() != 1)
  {
    return;
  }
  asset::model::Asset *asset = entries[0]->AsAsset();
  if (!asset)
  {
    return;
  }

  const cs::Class *cls = asset->GetClass();
  if (!cls)
  {
    return;
  }

  
  for (const cs::Class *validClass : m_validClasses)
  {
    if (cls->IsInstanceOf(validClass))
    {
      event->accept();
      return;
    }
  }
}


void AssetResourceLineEdit::get(QDataStream &stream, std::vector<asset::model::Entry*> &entries) const
{
  quint32 numEntries;
  stream >> numEntries;
  for (quint32 i = 0; i < numEntries; ++i)
  {
    quint64 ptr;
    stream >> ptr;
    asset::model::Entry *entry = reinterpret_cast<asset::model::Entry*>(ptr);
    entries.push_back(entry);
  }
}


void AssetResourceLineEdit::dropEvent(QDropEvent *event)
{
  const QMimeData *mimeData = event->mimeData();
  if (!mimeData->hasFormat("application/assetModelEntryPtr"))
  {
    return;
  }
  QByteArray rawData = mimeData->data("application/assetModelEntryPtr");
  QDataStream entriesStream(&rawData, QIODevice::ReadOnly);
  std::vector<asset::model::Entry*> entries;
  get(entriesStream, entries);
  if (entries.size() != 1)
  {
    return;
  }
  asset::model::Entry *entry = entries[0];

  emit ResourceChanged(entry->GetResourceLocator());
}


void AssetResourceLineEdit::AddValidClass(const cs::Class *cls)
{
  m_validClasses << cls;
}

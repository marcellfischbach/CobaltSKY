
#include <components/assetresourcelineedit.hh>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <mimehelper.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcelocator.hh>

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
  if (!MimeHelper::HasClass(mimeData))
  {
    return;
  }
  const vkClass *cls = MimeHelper::GetClass(mimeData);
  if (!cls)
  {
    return;
  }
  for (const vkClass *validClass : m_validClasses)
  {
    if (cls->IsInstanceOf(validClass))
    {
      event->accept();
      return;
    }
  }
}

void AssetResourceLineEdit::dropEvent(QDropEvent *event)
{
  const QMimeData *data = event->mimeData();
  if (!MimeHelper::HasResourceLocator(data))
  {
    return;
  }

  vkResourceLocator locator = MimeHelper::GetResourceLocator(data);
  emit ResourceChanged(locator);
}


void AssetResourceLineEdit::AddValidClass(const vkClass *cls)
{
  m_validClasses << cls;
}
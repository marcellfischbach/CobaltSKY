
#include <mimehelper.hh>
#include <valkyrie/core/csresourcelocator.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/core/csclassregistry.hh>
#include <QMimeData>

#define CS_RESOURCE_LOCATOR_MIME_TYPE "cs-application/resource_locator"
#define CS_RESOURCE_TYPE_MIME_TYPE "cs-application/resource_type"
#define CS_CLASS_MIME_TYPE "cs-application/class"

csString MimeHelper::GetResourceLocatorMimeType()
{
  return CS_RESOURCE_LOCATOR_MIME_TYPE;
}

void MimeHelper::PutResourceLocatorMimeData(QMimeData *mimeData, const csResourceLocator &locator)
{
  if (!mimeData)
  {
    return;
  }
  mimeData->setData(
    QString(CS_RESOURCE_LOCATOR_MIME_TYPE),
    QString(locator.GetResourceFile().c_str()).toLatin1());
}

void MimeHelper::PutResourceLocatorMimeData(QMimeData *mimeData, const csString &resourceName)
{
  if (!mimeData)
  {
    return;
  }
  mimeData->setData(
    QString(CS_RESOURCE_LOCATOR_MIME_TYPE),
    QString(resourceName.c_str()).toLatin1());

}

bool MimeHelper::HasResourceLocator(const QMimeData *mimeData)
{
  if (!mimeData)
  {
    return false;
  }
  return mimeData->hasFormat(CS_RESOURCE_LOCATOR_MIME_TYPE);
}

csResourceLocator MimeHelper::GetResourceLocator(const QMimeData *mimeData, const csString &resource)
{
  QByteArray ba = mimeData->data(CS_RESOURCE_LOCATOR_MIME_TYPE);
  QString name(ba);

  return csResourceLocator(
    csString((const char*)name.toLatin1()),
    resource);
}

csString MimeHelper::GetResourceTypeMimeType()
{
  return CS_RESOURCE_TYPE_MIME_TYPE;
}

void MimeHelper::PutResourceTypeMimeData(QMimeData *mimeData, const csString &type)
{
  mimeData->setData(
    QString(CS_RESOURCE_TYPE_MIME_TYPE),
    QString(type.c_str()).toLatin1());
  
}

bool MimeHelper::HasResourceType(const QMimeData *data)
{
  if (!data)
  {
    return false;
  }

  return data->hasFormat(CS_RESOURCE_TYPE_MIME_TYPE);
}

csString MimeHelper::GetResourceType(const QMimeData *mimeData)
{
  QByteArray ba = mimeData->data(CS_RESOURCE_LOCATOR_MIME_TYPE);
  QString name(ba);

  return csString((const char*)name.toLatin1());
}



csString MimeHelper::GetClassMimeType()
{
  return CS_CLASS_MIME_TYPE;
}

void MimeHelper::PutClassMimeData(QMimeData *mimeData, const csClass *cls)
{
  mimeData->setData(
    QString(CS_CLASS_MIME_TYPE),
    QString(cls->GetName().c_str()).toLatin1());

}

bool MimeHelper::HasClass(const QMimeData *data)
{
  if (!data)
  {
    return false;
  }

  return data->hasFormat(CS_CLASS_MIME_TYPE);
}

const csClass *MimeHelper::GetClass(const QMimeData *mimeData)
{
  QByteArray ba = mimeData->data(CS_CLASS_MIME_TYPE);
  QString name(ba);

  return csClassRegistry::Get()->GetClass(csString((const char*)name.toLatin1()));
}

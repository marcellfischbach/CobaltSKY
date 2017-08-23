
#include <mimehelper.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csclassregistry.hh>
#include <QMimeData>

#define CS_RESOURCE_LOCATOR_MIME_TYPE "cs-application/resource_locator"
#define CS_RESOURCE_TYPE_MIME_TYPE "cs-application/resource_type"
#define CS_CLASS_MIME_TYPE "cs-application/class"

std::string MimeHelper::GetResourceLocatorMimeType()
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

void MimeHelper::PutResourceLocatorMimeData(QMimeData *mimeData, const std::string &resourceName)
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

csResourceLocator MimeHelper::GetResourceLocator(const QMimeData *mimeData, const std::string &resource)
{
  QByteArray ba = mimeData->data(CS_RESOURCE_LOCATOR_MIME_TYPE);
  QString name(ba);

  return csResourceLocator(
    std::string((const char*)name.toLatin1()),
    resource);
}

std::string MimeHelper::GetResourceTypeMimeType()
{
  return CS_RESOURCE_TYPE_MIME_TYPE;
}

void MimeHelper::PutResourceTypeMimeData(QMimeData *mimeData, const std::string &type)
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

std::string MimeHelper::GetResourceType(const QMimeData *mimeData)
{
  QByteArray ba = mimeData->data(CS_RESOURCE_LOCATOR_MIME_TYPE);
  QString name(ba);

  return std::string((const char*)name.toLatin1());
}



std::string MimeHelper::GetClassMimeType()
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

  return csClassRegistry::Get()->GetClass(std::string((const char*)name.toLatin1()));
}

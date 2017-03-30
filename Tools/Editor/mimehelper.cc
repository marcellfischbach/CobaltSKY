
#include <mimehelper.hh>
#include <valkyrie/core/vkresourcelocator.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkclassregistry.hh>
#include <QMimeData>

#define VK_RESOURCE_LOCATOR_MIME_TYPE "vk-application/resource_locator"
#define VK_RESOURCE_TYPE_MIME_TYPE "vk-application/resource_type"
#define VK_CLASS_MIME_TYPE "vk-application/class"

vkString MimeHelper::GetResourceLocatorMimeType()
{
  return VK_RESOURCE_LOCATOR_MIME_TYPE;
}

void MimeHelper::PutResourceLocatorMimeData(QMimeData *mimeData, const vkResourceLocator &locator)
{
  if (!mimeData)
  {
    return;
  }
  mimeData->setData(
    QString(VK_RESOURCE_LOCATOR_MIME_TYPE),
    QString(locator.GetResourceFile().c_str()).toLatin1());
}

void MimeHelper::PutResourceLocatorMimeData(QMimeData *mimeData, const vkString &resourceName)
{
  if (!mimeData)
  {
    return;
  }
  mimeData->setData(
    QString(VK_RESOURCE_LOCATOR_MIME_TYPE),
    QString(resourceName.c_str()).toLatin1());

}

bool MimeHelper::HasResourceLocator(const QMimeData *mimeData)
{
  if (!mimeData)
  {
    return false;
  }
  return mimeData->hasFormat(VK_RESOURCE_LOCATOR_MIME_TYPE);
}

vkResourceLocator MimeHelper::GetResourceLocator(const QMimeData *mimeData, const vkString &resource)
{
  QByteArray ba = mimeData->data(VK_RESOURCE_LOCATOR_MIME_TYPE);
  QString name(ba);

  return vkResourceLocator(
    vkString((const char*)name.toLatin1()),
    resource);
}

vkString MimeHelper::GetResourceTypeMimeType()
{
  return VK_RESOURCE_TYPE_MIME_TYPE;
}

void MimeHelper::PutResourceTypeMimeData(QMimeData *mimeData, const vkString &type)
{
  mimeData->setData(
    QString(VK_RESOURCE_TYPE_MIME_TYPE),
    QString(type.c_str()).toLatin1());
  
}

bool MimeHelper::HasResourceType(const QMimeData *data)
{
  if (!data)
  {
    return false;
  }

  return data->hasFormat(VK_RESOURCE_TYPE_MIME_TYPE);
}

vkString MimeHelper::GetResourceType(const QMimeData *mimeData)
{
  QByteArray ba = mimeData->data(VK_RESOURCE_LOCATOR_MIME_TYPE);
  QString name(ba);

  return vkString((const char*)name.toLatin1());
}



vkString MimeHelper::GetClassMimeType()
{
  return VK_CLASS_MIME_TYPE;
}

void MimeHelper::PutClassMimeData(QMimeData *mimeData, const vkClass *cls)
{
  mimeData->setData(
    QString(VK_CLASS_MIME_TYPE),
    QString(cls->GetName().c_str()).toLatin1());

}

bool MimeHelper::HasClass(const QMimeData *data)
{
  if (!data)
  {
    return false;
  }

  return data->hasFormat(VK_CLASS_MIME_TYPE);
}

const vkClass *MimeHelper::GetClass(const QMimeData *mimeData)
{
  QByteArray ba = mimeData->data(VK_CLASS_MIME_TYPE);
  QString name(ba);

  return vkClassRegistry::Get()->GetClass(vkString((const char*)name.toLatin1()));
}

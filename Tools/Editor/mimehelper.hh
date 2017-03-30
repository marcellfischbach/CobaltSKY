#pragma once

#include <valkyrie/core/vkstring.hh>

class vkResourceLocator;
class QMimeData;
class vkClass;

class MimeHelper
{
public:
  static vkString GetResourceLocatorMimeType();
  static void PutResourceLocatorMimeData(QMimeData *mimeData, const vkResourceLocator &locator);
  static void PutResourceLocatorMimeData(QMimeData * mimeData, const vkString &locatorName);
  static bool HasResourceLocator(const QMimeData *data);
  static vkResourceLocator GetResourceLocator(const QMimeData *data, const vkString &resource = "");

  static vkString GetResourceTypeMimeType();
  static void PutResourceTypeMimeData(QMimeData *mimeData, const vkString &type);
  static bool HasResourceType(const QMimeData *data);
  static vkString GetResourceType(const QMimeData *data);

  static vkString GetClassMimeType();
  static void PutClassMimeData(QMimeData *mimeData, const vkClass *cls);
  static bool HasClass(const QMimeData *data);
  static const vkClass *GetClass(const QMimeData *data);

};
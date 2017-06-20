#pragma once

#include <cobalt/core/csstring.hh>

class csResourceLocator;
class QMimeData;
class csClass;

class MimeHelper
{
public:
  static csString GetResourceLocatorMimeType();
  static void PutResourceLocatorMimeData(QMimeData *mimeData, const csResourceLocator &locator);
  static void PutResourceLocatorMimeData(QMimeData * mimeData, const csString &locatorName);
  static bool HasResourceLocator(const QMimeData *data);
  static csResourceLocator GetResourceLocator(const QMimeData *data, const csString &resource = "");

  static csString GetResourceTypeMimeType();
  static void PutResourceTypeMimeData(QMimeData *mimeData, const csString &type);
  static bool HasResourceType(const QMimeData *data);
  static csString GetResourceType(const QMimeData *data);

  static csString GetClassMimeType();
  static void PutClassMimeData(QMimeData *mimeData, const csClass *cls);
  static bool HasClass(const QMimeData *data);
  static const csClass *GetClass(const QMimeData *data);

};
#pragma once

#include <editor/editorexport.hh>
#include <string>

class csResourceLocator;
class QMimeData;
class cs::Class;

class MimeHelper
{
public:
  static std::string GetResourceLocatorMimeType();
  static void PutResourceLocatorMimeData(QMimeData *mimeData, const csResourceLocator &locator);
  static void PutResourceLocatorMimeData(QMimeData * mimeData, const std::string &locatorName);
  static bool HasResourceLocator(const QMimeData *data);
  static csResourceLocator GetResourceLocator(const QMimeData *data, const std::string &resource = "");

  static std::string GetResourceTypeMimeType();
  static void PutResourceTypeMimeData(QMimeData *mimeData, const std::string &type);
  static bool HasResourceType(const QMimeData *data);
  static std::string GetResourceType(const QMimeData *data);

  static std::string GetClassMimeType();
  static void PutClassMimeData(QMimeData *mimeData, const cs::Class *cls);
  static bool HasClass(const QMimeData *data);
  static const cs::Class *GetClass(const QMimeData *data);

};
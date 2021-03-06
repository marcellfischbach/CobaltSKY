#pragma once


#include <editor/editorexport.hh>
#include <QString>
#include <QPixmap>
#include <cobalt/core/csresourcelocator.hh>


class AssetManagerContentModelEntry
{
public:
  AssetManagerContentModelEntry(const cs::ResourceLocator &locator);

  void ReloadIcon();

  const QPixmap &GetIcon() const
  {
    return m_pixmap;
  }

  const QString &GetEntryName() const
  {
    return m_entryName;
  }

  const QString &GetTypeName() const
  {
    return m_typeName;
  }

  const cs::ResourceLocator &GetLocator() const
  {
    return m_locator;
  }

private:
  QString m_entryName;
  QString m_typeName;
  QPixmap m_pixmap;
  cs::ResourceLocator m_locator;
};

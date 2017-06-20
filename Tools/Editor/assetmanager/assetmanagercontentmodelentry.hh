#pragma once


#include <QString>
#include <QPixmap>
#include <cobalt/core/csresourcelocator.hh>


class AssetManagerContentModelEntry
{
public:
  AssetManagerContentModelEntry(const csResourceLocator &locator);

  const QPixmap &GetIcon() const
  {
    return m_pixmap;
  }

  const QString &GetEntryName() const
  {
    return m_entryName;
  }

  const csResourceLocator &GetLocator() const
  {
    return m_locator;
  }

private:
  QString m_entryName;
  QPixmap m_pixmap;
  csResourceLocator m_locator;
};

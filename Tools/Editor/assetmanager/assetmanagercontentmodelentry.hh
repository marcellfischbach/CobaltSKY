#pragma once


#include <QString>
#include <QPixmap>
#include <valkyrie/core/vkresourcelocator.hh>


class AssetManagerContentModelEntry
{
public:
  AssetManagerContentModelEntry(const vkResourceLocator &locator);

  const QPixmap &GetIcon() const
  {
    return m_pixmap;
  }

  const QString &GetEntryName() const
  {
    return m_entryName;
  }

  const vkResourceLocator &GetLocator() const
  {
    return m_locator;
  }

private:
  QString m_entryName;
  QPixmap m_pixmap;
  vkResourceLocator m_locator;
};

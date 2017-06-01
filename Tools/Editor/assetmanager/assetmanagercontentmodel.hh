#pragma once

#include <QAbstractItemModel>
#include <QDir>
#include <QList>
#include <valkyrie/core/vkstring.hh>
#include <valkyrie/core/vkresourcelocator.hh>

class AssetManagerContentModelEntry;
class vkClass;

class AssetManagerContentModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  AssetManagerContentModel();
  virtual ~AssetManagerContentModel();

  void SetResourceLocator(const vkResourceLocator &locator);

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual QStringList mimeTypes() const;
  virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
  vkResourceLocator GetLocator(const QModelIndex &index) const;
private:
  void CleanupEntries();
  vkString ReadType(const vkResourceLocator &fileName) const;

  vkResourceLocator m_locator;
  QList<AssetManagerContentModelEntry*> m_entries;
};

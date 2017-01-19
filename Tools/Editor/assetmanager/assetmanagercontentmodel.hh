#pragma once

#include <QAbstractItemModel>
#include <QDir>
#include <QList>
#include <valkyrie/core/vkstring.hh>

class AssetManagerContentModelEntry;

class AssetManagerContentModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  AssetManagerContentModel();
  virtual ~AssetManagerContentModel();

  void SetDir(const QDir &dir);

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  vkString GetEntry(const QModelIndex &index) const;
private:
  void CleanupEntries();

  QDir m_dir;
  QList<AssetManagerContentModelEntry*> m_entries;
};
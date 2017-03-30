#pragma once

#include <QAbstractItemModel>
#include <QDir>
#include <QList>
#include <valkyrie/core/vkstring.hh>

class AssetManagerContentModelEntry;
class vkClass;

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
  virtual QStringList mimeTypes() const;
  virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
  vkString GetEntry(const QModelIndex &index) const;
private:
  void CleanupEntries();
  vkString ExtractResourceName(const vkString &fileName) const;
  vkString ReadType(const vkString &fileName) const;
  const vkClass *ReadClass(const vkString &fileName) const;

  QDir m_dir;
  QList<AssetManagerContentModelEntry*> m_entries;
};

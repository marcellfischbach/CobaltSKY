#pragma once

#include <QAbstractItemModel>
#include <QDir>
#include <QList>
#include <cobalt/core/csstring.hh>
#include <cobalt/core/csresourcelocator.hh>

class AssetManagerContentModelEntry;
class csClass;

class AssetManagerContentModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  AssetManagerContentModel();
  virtual ~AssetManagerContentModel();

  void SetResourceLocator(const csResourceLocator &locator);
  const csResourceLocator &GetResourceLocator() const;
  void Refresh();

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual QStringList mimeTypes() const;
  virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
  csResourceLocator GetLocator(const QModelIndex &index) const;

  const AssetManagerContentModelEntry *GetEntry(const QModelIndex &index) const;
  void PreviewIconChanged(const csResourceLocator &locator);
private:
  void CleanupEntries();
  csString ReadType(const csResourceLocator &fileName) const;

  csResourceLocator m_locator;
  QList<AssetManagerContentModelEntry*> m_entries;
};

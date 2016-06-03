#pragma once

#include <qabstractitemmodel.h>
#include <qdir.h>
#include <Valkyrie/Core/ResourceManager.hh>
#include <AssetManager/EditorIcon.hh>


namespace assetmanager
{


class FolderItemModel : public QAbstractItemModel
{
public:

  FolderItemModel();
  virtual ~FolderItemModel();

  void Refresh();
  void SetPath(const QDir &path);
  const QDir &GetPath() const;

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
  virtual QMimeData *mimeData(const QModelIndexList &indexes) const;

  vkResourceLocator CreateLocator (const QString &assetName) const;
  vkResourceLocator GetLocator(const QModelIndex &index, const vkString &name = "") const;

  bool IsContainer(const QModelIndex &index);
  bool IsContainerOpen(const QModelIndex &index);
  void CloseContainer(const QModelIndex &index);
  void OpenContainer(const QModelIndex &index);


private:
  void Clear();
  struct Entry
  {
    QString name;
    QString displayName;
    EditorIcon *icon;
    QString resourceString;
    bool container;
    bool containerOpen;
    Entry *parent;
    Entry () { }
  };


  vkResourceLocator GetLocator(Entry *entry, const vkString &name = "DATA") const;

  QDir m_path;
  QList<Entry*> m_entries;
  QString m_resourceRel;
};


inline const QDir &FolderItemModel::GetPath() const
{
  return m_path;
}


}


#pragma once 

#include <editor/editorexport.hh>
#include <QAbstractItemModel>

#include <cobalt/core/csresourcelocator.hh>

namespace asset::model
{
  class Entry;
  class ViewEntry;
  class ViewFolder;
  class ViewRoot;
  class ViewDataModel;

  class TreeModel : public QAbstractItemModel
  {
    Q_OBJECT
  public:
    TreeModel();
    virtual ~TreeModel();

    void SetViewDataModel(ViewDataModel *dataModel);

    QModelIndex GetRootModelIndex() const;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    ViewEntry *GetEntry(const QModelIndex &index);
    const ViewEntry *GetEntry(const QModelIndex &index) const;

		Qt::DropActions supportedDropActions() const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		QStringList mimeTypes() const;
		QMimeData *mimeData(const QModelIndexList &indices) const;
		bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;
		bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
  private slots:
    void EntryAdded(asset::model::ViewEntry *parent, asset::model::ViewEntry *child);
    void EntryAboutToRemove(asset::model::ViewEntry *parent, asset::model::ViewEntry *child);
    void EntryRemoved(asset::model::ViewEntry *parent, asset::model::ViewEntry *child);


  private:
		void put(QDataStream &stream, const std::vector<Entry*> &entries) const;
		void get(QDataStream &stream, std::vector<Entry*> &entries) const;
    void ConnectDataModel();
    void DisconnectDataModel();

    ViewEntry * GetChildOfFolder(ViewFolder *folder, size_t idx) const;
    ViewEntry * GetChildOfRoot(ViewRoot *root, size_t idx) const;
    int IndexOf(ViewEntry *parent, ViewEntry *child) const;
    int IndexOfRootParent(ViewRoot *root, ViewEntry *child) const;
    int IndexOfFolderParent(ViewFolder *folder, ViewEntry *child) const;


    ViewDataModel * m_dataModel;
  };

}
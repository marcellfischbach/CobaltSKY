
#pragma once 

#include <QAbstractItemModel>

#include <cobalt/core/csresourcelocator.hh>

namespace asset::model
{
	class Entry;
  class ViewEntry;
  class ViewFolder;
  class ViewRoot;
  class ViewDataModel;

  class FolderTreeModel : public QAbstractItemModel
  {
    Q_OBJECT
  public:
    FolderTreeModel();
    virtual ~FolderTreeModel();

    void SetViewDataModel(ViewDataModel *dataModel);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

		ViewEntry *GetEntry(const QModelIndex &index);
		const ViewEntry *GetEntry(const QModelIndex &index) const;
		

  private:

    ViewEntry * GetChild(ViewEntry *entry, size_t idx) const;
    int IndexOf(ViewEntry *parent, ViewEntry *child) const;


    ViewDataModel * m_dataModel;
  };

}
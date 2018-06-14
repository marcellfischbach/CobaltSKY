
#pragma once

#include <QAbstractItemModel>
#include <cobalt/core/csresourcelocator.hh>

namespace asset::model
{
	class ViewEntry;
	class ViewFolder;
	class ViewDataModel;
	class AssetListModel : public QAbstractItemModel
	{
		Q_OBJECT;
	public:
		AssetListModel(ViewDataModel *dataModel);
		virtual ~AssetListModel();

		void SetFolder(ViewFolder *folder);
		ViewFolder *GetFolder();

    csResourceLocator GetResourceLocator() const;

		virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		virtual QModelIndex parent(const QModelIndex &child) const;
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

		ViewEntry *GetEntry(const QModelIndex &index) const;

	private slots:
		void EntryAdded(asset::model::ViewEntry *parent, asset::model::ViewEntry *child);

	private:
		ViewFolder * m_folder;
		ViewDataModel *m_dataModel;
	};


	
}

inline asset::model::ViewFolder *asset::model::AssetListModel::GetFolder()
{
	return m_folder;
}


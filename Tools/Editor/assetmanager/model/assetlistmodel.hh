
#pragma once

#include <QAbstractItemModel>

namespace asset::model
{
	class ViewFolder;
	class AssetListModel : public QAbstractItemModel
	{
	public:
		AssetListModel();
		virtual ~AssetListModel();

		void SetFolder(ViewFolder *folder);

		virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		virtual QModelIndex parent(const QModelIndex &child) const;
		virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
		virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	private:
		ViewFolder * m_folder;

	};

}


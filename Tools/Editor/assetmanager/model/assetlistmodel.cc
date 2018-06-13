
#include <assetmanager/model/assetlistmodel.hh>
#include <assetmanager/model/viewasset.hh>
#include <assetmanager/model/viewfolder.hh>
#include <assetmodel/entry.hh>



namespace asset::model
{

	AssetListModel::AssetListModel()
		: QAbstractItemModel()
		, m_folder(0)
	{

	}

	AssetListModel::~AssetListModel()
	{

	}

	void AssetListModel::SetFolder(ViewFolder *folder)
	{
		if (folder != m_folder)
		{
			beginResetModel();
			m_folder = folder;
			endResetModel();
		}
	}



	QModelIndex AssetListModel::index(int row, int column, const QModelIndex &parent) const
	{
		if (parent.isValid())
		{
			return QModelIndex();
		}
		if (!m_folder)
		{
			return QModelIndex();
		}

		if (row >= m_folder->GetAssets().size())
		{
			return QModelIndex();
		}
		return createIndex(row, column, m_folder->GetAssets()[row]);
	}

	QModelIndex AssetListModel::parent(const QModelIndex &child) const
	{
		return QModelIndex();
	}

	int AssetListModel::rowCount(const QModelIndex &parent) const
	{
		if (parent.isValid())
		{
			return 0;
		}
		if (!m_folder)
		{
			return 0;
		}
		return m_folder->GetAssets().size();
	}

	int AssetListModel::columnCount(const QModelIndex &parent) const
	{
		return 1;
	}

	QVariant AssetListModel::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid() || role != Qt::DisplayRole)
		{
			return QVariant();
		}

		const ViewEntry *entry = reinterpret_cast<const ViewEntry*>(index.internalPointer());
		if (!entry)
		{
			return QVariant();
		}

		return QString(entry->GetEntry()->GetName().c_str());
	}





}


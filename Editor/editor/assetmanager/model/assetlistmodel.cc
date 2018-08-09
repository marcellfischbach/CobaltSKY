
#include <editor/assetmanager/model/assetlistmodel.hh>
#include <editor/assetmanager/model/viewasset.hh>
#include <editor/assetmanager/model/viewfolder.hh>
#include <editor/assetmanager/model/viewdatamodel.hh>
#include <editor/assetmodel/entry.hh>
#include <editor/assetmodel/folder.hh>



namespace asset::model
{

	AssetListModel::AssetListModel(ViewDataModel *dataModel)
		: QAbstractItemModel()
		, m_folder(0)
		, m_dataModel(dataModel)
	{
		connect(m_dataModel, SIGNAL(EntryAdded(asset::model::ViewEntry*, asset::model::ViewEntry*)), 
			this, SLOT(EntryAdded(asset::model::ViewEntry *, asset::model::ViewEntry*)));


    connect(m_dataModel, SIGNAL(EntryAboutToRemove(asset::model::ViewEntry*, asset::model::ViewEntry*)),
      this, SLOT(EntryAboutToRemove(asset::model::ViewEntry *, asset::model::ViewEntry*)));
    connect(m_dataModel, SIGNAL(EntryRemoved(asset::model::ViewEntry*, asset::model::ViewEntry*)),
      this, SLOT(EntryRemoved(asset::model::ViewEntry *, asset::model::ViewEntry*)));
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

  csResourceLocator  AssetListModel::GetResourceLocator() const
  {
    if (!m_folder)
    {
      return csResourceLocator();
    }

    return m_folder->GetFolder()->GetResourceLocator();
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
		return QString(entry->GetEntry ()->GetName().c_str());
	}



	ViewEntry *AssetListModel::GetEntry(const QModelIndex &index) const
	{
		if (!index.isValid())
		{
			return 0;
		}

		return reinterpret_cast<ViewEntry*>(index.internalPointer());
	}




	void AssetListModel::EntryAdded(ViewEntry *parent, ViewEntry *child)
	{
		if (parent != m_folder)
		{
			return;
		}

		for (size_t i = 0, in = m_folder->GetAssets().size(); i < in; ++i)
		{
			if (m_folder->GetAssets()[i] == child)
			{
				beginInsertRows(QModelIndex(), i, i);
				endInsertRows();
				return;
			}
		}
	}

  void AssetListModel::EntryAboutToRemove(ViewEntry *parent, ViewEntry *child)
  {
    if (parent != m_folder)
    {
      return;
    }

    const std::vector<asset::model::ViewEntry*> &assets = m_folder->GetAssets();
    auto it = std::find(assets.begin(), assets.end(), child);
    if (it == assets.end())
    {
      return;
    }

    std::ptrdiff_t pos = std::distance(assets.begin(), it);
    beginRemoveRows(QModelIndex(), pos, pos);
  }


  void AssetListModel::EntryRemoved(ViewEntry *parent, ViewEntry *child)
  {
    if (parent != m_folder)
    {
      return;
    }

    endRemoveRows();

  }

}



#include <assetmanager/model/viewdatamodel.hh>
#include <assetmanager/model/viewasset.hh>
#include <assetmanager/model/viewfolder.hh>
#include <assetmanager/model/viewroot.hh>
#include <assetmanager/model/viewvfsentry.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>
#include <assetmodel/root.hh>
#include <assetmodel/vfsentry.hh>



namespace asset::model
{

  ViewDataModel::ViewDataModel()
    : QObject()
    , m_model(0)
    , m_root(0)
  {

  }

  ViewDataModel::~ViewDataModel()
  {

  }


  void ViewDataModel::SetModel(Model *model)
  {
		if (m_model)
		{
			Disconnect();
		}
    m_model = model;
    Rebuild();
		if (m_model)
		{
			Connect();
		}
  }

	void ViewDataModel::Connect()
	{
		connect(m_model, SIGNAL(EntryAdded(asset::model::Entry *, asset::model::Entry *)),
			this, SLOT(EntryAdded(asset::model::Entry *, asset::model::Entry *)));
	}

	void ViewDataModel::Disconnect()
	{
		disconnect(m_model, SIGNAL(EntryAdded(asset::model::Entry *, asset::model::Entry *)),
			this, SLOT(EntryAdded(asset::model::Entry *, asset::model::Entry *)));
	}

  void ViewDataModel::Cleanup()
  {
    if (m_root)
    {
      delete m_root;
      m_root = 0;
    }

    m_data.clear();

  }

  void ViewDataModel::Rebuild()
  {
    Cleanup();
    if (!m_model)
    {
      return;
    }
    m_root = static_cast<ViewRoot*>(Build(m_model->GetRoot ()));
  }

  ViewEntry *ViewDataModel::Build(Entry *entry)
  {
    ViewEntry *viewEntry = Create(entry);
    if (!viewEntry)
    {
      return 0;
    }

    for (Entry *childEntry: entry->GetChildren())
    {
      ViewEntry *viewChildEntry = Build(childEntry);
      if (viewChildEntry)
      {
        viewEntry->Add(viewChildEntry);
      }
    }

    viewEntry->Sort();

    return viewEntry;
  }


  ViewEntry *ViewDataModel::Create(Entry *entry)
  {
    if (!entry)
    {
      return 0;
    }

		ViewEntry *viewEntry;
    switch (entry->GetType())
    {
    case Entry::eT_Asset:
      viewEntry = new ViewAsset(static_cast<Asset*>(entry));
			break;
    case Entry::eT_Folder:
      viewEntry = new ViewFolder(static_cast<Folder*>(entry));
			break;
    case Entry::eT_VFSEntry:
      viewEntry = new ViewVFSEntry(static_cast<VFSEntry*>(entry));
			break;
    case Entry::eT_Root:
      viewEntry = new ViewRoot(static_cast<Root*>(entry));
			break;
    }
		if (viewEntry)
		{
			m_data[entry] = viewEntry;
		}
		return viewEntry;
  }



	void ViewDataModel::EntryAdded(Entry *parent, Entry *child)
	{
		auto parentIt = m_data.find(parent);
		if (parentIt == m_data.end())
		{
			return;
		}

		ViewEntry *viewParent = parentIt->second;

		ViewEntry *viewChild = 0;
		auto childIt = m_data.find(child);
		if (childIt == m_data.end())
		{
			viewChild = Create(child);
		}
		else
		{
			viewChild = childIt->second;
		}

		
		if (viewParent && viewChild)
		{
			emit EntryAboutToAdd(viewParent, viewChild);
			viewParent->Add(viewChild);
			emit EntryAdded(viewParent, viewChild);
		}

	}


}
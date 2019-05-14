
#include <editor/assetmodel/asset.hh>
#include <editor/assetmodel/model.hh>
#include <cobalt/core/csresourcemanager.hh>

namespace asset::model
{
  Asset::Asset(Model *model, const std::string &fileName)
    : Entry(model, eT_Asset)
    , m_assetName("")
    , m_assetType("undef")
    , m_cls(0)
  {
    SetName(fileName);
  }

  Asset::~Asset()
  {

  }

	void Asset::Add(Entry *entry)
	{
		// assets never have children
	}

	void Asset::SetName(const std::string &name)
	{
		std::string fileName = name;
		if (name.find(".asset") == name.length() - 6)
		{
			// this is the filename
			m_assetName = name.substr(0, name.length() - 6);
			fileName = name;
		}
		else
		{ 
			m_assetName = name;
			fileName = name + ".asset";
		}

		Entry::SetName(fileName);
	}

	const std::string &Asset::GetDisplayName() const
	{
		return m_assetName;
	}

	const std::string Asset::FakeName(const std::string &name) const
	{
		if (name.find(".asset") == name.length() - 6)
		{
			return name;
		}
		return name + ".asset";
	}


  csResourceLocator Asset::GetResourceLocator() const
  {
		const Entry *parent = GetParent();
		if (!parent)
		{
			return csResourceLocator();
		}

		csResourceLocator parentLocator = parent->GetResourceLocator();
		if (!parentLocator.IsValid())
		{
			return csResourceLocator();
		}
		return parent->GetResourceLocator().WithFileSuffix(GetName());
	}



	bool Asset::IsAsset() const
	{
		return true;
	}

	Asset *Asset::AsAsset()
	{
		return this;
	}

	const	Asset *Asset::AsAsset() const
	{
		return this;
	}

	void Asset::ClearReferences()
	{
		GetModel()->ClearReferences(this);
	}

	void Asset::AddReference(const csResourceLocator &locator)
	{
		GetModel()->AddReference(this, locator);
	}

  const cs::Class *Asset::GetClass() const
  {
    if (!m_cls)
    {
      m_cls = csResourceManager::Get()->EvalClass(GetResourceLocator());
    }
    return m_cls;
  }
}


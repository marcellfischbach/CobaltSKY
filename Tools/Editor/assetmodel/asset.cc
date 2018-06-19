
#include <assetmodel/asset.hh>
#include <assetmodel/model.hh>

namespace asset::model
{
  Asset::Asset(Model *model, const std::string &fileName)
    : Entry(model, eT_Asset)
    , m_fileName(fileName)
    , m_assetType("undef")
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
		if (name.find(".asset") != name.length() - 6)
		{
			m_fileName = name + ".asset";
			Entry::SetName(name);
		}
		else
		{ 
			m_fileName = name;
			Entry::SetName(name.substr(0, name.length() - 6));
		}

	}

  void Asset::ClearReferences()
  {
    m_references.clear();
    GetModel()->RemoveReferences(this);
  }

  void Asset::AddReference(const csResourceLocator &reference)
  {
    m_references.insert(reference);
    GetModel()->AddReference(this, reference);
  }

  void Asset::RenameReference(const csResourceLocator &oldLocator, const csResourceLocator &newLocator)
  {
    auto it = m_references.find(oldLocator);
    if (it != m_references.end())
    {
      m_references.erase(it);
      m_references.insert(newLocator);
    }

  }


  csResourceLocator Asset::GetResourceLocator() const
  {
		const Entry *parent = GetParent();
		if (parent)
		{
			return parent->GetResourceLocator().WithFileSuffix(m_fileName);
		}
		return csResourceLocator();
  }

	csResourceLocator Asset::GetNamedResourceLocator(const std::string &name) const
	{
		std::string fileName = name;
		if (name.find(".asset") != name.length() - 6)
		{
			fileName = name + ".asset";
		}


		const Entry *parent = GetParent();
		if (parent)
		{
			return parent->GetResourceLocator().WithFileSuffix(fileName);
		}
		return csResourceLocator();
	}

	csResourceLocator Asset::Construct(const csResourceLocator &parentLocator) const
	{
		return parentLocator.WithFileSuffix(m_fileName);
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
}


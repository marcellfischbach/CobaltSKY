
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
    m_references.push_back(reference);

    GetModel()->AddReference(this, reference);
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


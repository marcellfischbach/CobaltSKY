
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>

namespace asset::model
{
  Folder::Folder(Model *model, const std::string &folderName)
    : Entry(model, Entry::eT_Folder)
  {
    SetName(folderName);
  }

  Folder::Folder(Model *model, Entry::Type type)
    : Entry(model, type)
  {

  }

  Folder::~Folder()
  {

  }

	csResourceLocator Folder::GetResourceLocator() const
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
		return parent->GetResourceLocator().WithFileSuffix(GetName() + "/");
	}

	
  void Folder::Add(Entry *entry, ModelTransaction &tr)
  {
    if (!entry)
    {
      return;
    }

    if (entry->GetType() == Entry::eT_Folder || entry->GetType() == Entry::eT_Asset)
    {
      Entry::Add(entry, tr);
    }
  }

	const std::string Folder::GetNewAssetName(const std::string &baseName) const
	{
		std::string testName = baseName;
		if (!GetChildByName(testName))
		{
			return testName + ".asset";
		}
		for (unsigned i = 1; i < 100; ++i)
		{
			testName = baseName + std::to_string(i);
			if (!GetChildByName(testName))
			{
				return testName + ".asset";
			}
		}
		return std::string();
	}

	csResourceLocator Folder::GetNewResourceLocator(const std::string &baseName) const
	{
		std::string assetName = GetNewAssetName(baseName);
		if (assetName.empty())
		{
			return csResourceLocator();
		}
		return GetResourceLocator().WithFileSuffix(assetName);
	}

	bool Folder::IsFolder() const
	{
		return true;
	}

	Folder *Folder::AsFolder()
	{
		return this;
	}

	const	Folder *Folder::AsFolder() const
	{
		return this;
	}


}

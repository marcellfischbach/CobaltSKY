
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

  void Asset::ClearReferences()
  {
    m_references.clear();
  }

  void Asset::AddReference(const csResourceLocator &reference)
  {
    m_references.push_back(reference);
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
}


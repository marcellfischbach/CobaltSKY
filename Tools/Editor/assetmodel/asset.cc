
#include <assetmodel/asset.hh>
#include <assetmodel/model.hh>

namespace asset::model
{
  Asset::Asset(Model *model, const std::string &fileName)
    : Entry(model, eT_Asset)
    , m_fileName(fileName)
  {

  }

  Asset::~Asset()
  {

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


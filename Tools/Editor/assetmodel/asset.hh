
#pragma once

#include <assetmodel/entry.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <string>

namespace asset::model
{
  class Model;
  class Asset : public Entry
  {
  public:
    Asset(Model *model, const std::string &fileName);
    virtual ~Asset();

    virtual csResourceLocator GetResourceLocator() const;

  private:
    std::string m_fileName;
  };

}


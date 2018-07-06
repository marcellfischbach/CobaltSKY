
#pragma once

#include <assetmodel/entry.hh>

namespace asset::model
{
  class Model;
  class Root : public Entry
  {
  public:
    Root(Model *model);
    virtual ~Root();

    virtual void Add(Entry *entry, ModelTransaction &tr);
    virtual void Delete(ModelTransaction &tr);

    virtual bool IsAttached() const;

    virtual csResourceLocator GetResourceLocator() const;


  };

}
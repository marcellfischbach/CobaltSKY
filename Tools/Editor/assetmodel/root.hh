
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

    virtual void Add(Entry *entry);
    virtual void Delete();

    virtual bool IsAttached() const;

    virtual csResourceLocator GetResourceLocator() const;
  };

}

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

      virtual csResourceLocator GetResourceLocator() const;
    };

}
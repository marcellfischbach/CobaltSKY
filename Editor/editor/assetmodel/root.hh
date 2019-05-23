
#pragma once

#include <editor/editorexport.hh>
#include <editor/assetmodel/entry.hh>

namespace asset::model
{
  class Model;
  class EDITOR_API Root : public Entry
  {
  public:
    Root(Model *model);
    virtual ~Root();

    virtual void Add(Entry *entry, ModelTransaction &tr);
    virtual void Delete(ModelTransaction &tr);

    virtual bool IsAttached() const;

    virtual cs::ResourceLocator GetResourceLocator() const;

		virtual bool IsRoot() const;
		virtual Root *AsRoot();
		virtual const Root *AsRoot() const;
  };

}
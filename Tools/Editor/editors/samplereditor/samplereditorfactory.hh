#pragma once


#include <iasseteditorfactory.hh>


class SamplerEditorFactory : public iAssetEditorFactory
{
public:
  SamplerEditorFactory() { }
  virtual ~SamplerEditorFactory() { }

  virtual bool CanEdit(iObject *object, asset::model::Asset*asset) const;

  virtual iAssetEditor *CreateEditor(iObject *object, asset::model::Asset*asset) const;

};



#pragma once


#include <samplereditor/samplereditorexport.hh>
#include <editor/iasseteditorfactory.hh>


class SamplerEditorFactory : public iAssetEditorFactory
{
public:
  SamplerEditorFactory() { }
  virtual ~SamplerEditorFactory() { }

  virtual bool CanEdit(cs::iObject *object, asset::model::Asset*asset) const;

  virtual iAssetEditor *CreateEditor(cs::iObject *object, asset::model::Asset*asset) const;

};



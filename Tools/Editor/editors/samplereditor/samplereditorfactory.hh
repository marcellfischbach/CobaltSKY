#pragma once


#include <iasseteditorfactory.hh>


class SamplerEditorFactory : public iAssetEditorFactory
{
public:
  SamplerEditorFactory() { }
  virtual ~SamplerEditorFactory() { }

  virtual bool CanEdit(iObject *object, const AssetDescriptor &descriptor) const;

  virtual iAssetEditor *CreateEditor(iObject *object, const AssetDescriptor &descriptor) const;

};
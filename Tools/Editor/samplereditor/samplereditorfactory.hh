#pragma once


#include <iasseteditorfactory.hh>


class SamplerEditorFactory : public iAssetEditorFactory
{
public:
  SamplerEditorFactory() { }
  virtual ~SamplerEditorFactory() { }

  virtual bool CanEdit(const AssetDescriptor &descriptor) const;

  virtual iAssetEditor *CreateEditor(const AssetDescriptor &descriptor) const;

};
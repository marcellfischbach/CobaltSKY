#pragma once


#include <iasseteditorfactory.hh>


class TextureEditorFactory : public iAssetEditorFactory
{
public:
  TextureEditorFactory() { }
  virtual ~TextureEditorFactory() { }

  virtual bool CanEdit(const AssetDescriptor &descriptor) const;

  virtual iAssetEditor *CreateEditor(const AssetDescriptor &descriptor) const;

};
#pragma once


#include <iasseteditorfactory.hh>


class TextureEditorFactory : public iAssetEditorFactory
{
public:
  TextureEditorFactory() { }
  virtual ~TextureEditorFactory() { }

  virtual bool CanEdit(iObject *object, const AssetDescriptor &descriptor) const;

  virtual iAssetEditor *CreateEditor(iObject *object, const AssetDescriptor &descriptor) const;

};
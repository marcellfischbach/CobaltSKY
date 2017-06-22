#pragma once


#include <iasseteditorfactory.hh>


class MaterialEditorFactory : public iAssetEditorFactory
{
public:
  MaterialEditorFactory() { }
  virtual ~MaterialEditorFactory() { }

  virtual bool CanEdit(const AssetDescriptor &descriptor) const;

  virtual iAssetEditor *CreateEditor(const AssetDescriptor &descriptor) const;

};
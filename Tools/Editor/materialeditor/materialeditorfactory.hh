#pragma once


#include <iasseteditorfactory.hh>


class MaterialEditorFactory : public iAssetEditorFactory
{
public:
  MaterialEditorFactory() { }
  virtual ~MaterialEditorFactory() { }

  virtual bool CanEdit(iObject *object, const AssetDescriptor &descriptor) const;

  virtual iAssetEditor *CreateEditor(iObject *object, const AssetDescriptor &descriptor) const;

};
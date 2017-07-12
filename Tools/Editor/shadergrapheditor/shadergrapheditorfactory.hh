#pragma once


#include <iasseteditorfactory.hh>


class ShaderGraphEditorFactory : public iAssetEditorFactory
{
public:
  ShaderGraphEditorFactory() { }
  virtual ~ShaderGraphEditorFactory() { }

  virtual bool CanEdit(iObject *object, const AssetDescriptor &descriptor) const;

  virtual iAssetEditor *CreateEditor(iObject *object, const AssetDescriptor &descriptor) const;

};
#pragma once


#include <iasseteditorfactory.hh>


class ShaderGraphEditorFactory : public iAssetEditorFactory
{
public:
  ShaderGraphEditorFactory() { }
  virtual ~ShaderGraphEditorFactory() { }

  virtual bool CanEdit(const AssetDescriptor &descriptor) const;

  virtual iAssetEditor *CreateEditor(const AssetDescriptor &descriptor) const;

};
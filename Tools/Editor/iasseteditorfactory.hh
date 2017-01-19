#pragma once

#include <assetdescriptor.hh>


struct iAssetEditor;
struct iAssetEditorFactory
{

  virtual bool CanEdit(const AssetDescriptor &descriptor) const = 0;

  virtual iAssetEditor *CreateEditor(const AssetDescriptor &descriptor) const = 0;

};
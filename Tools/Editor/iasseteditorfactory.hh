#pragma once

#include <assetdescriptor.hh>
#include <cobalt/core/csclass.hh>

struct iAssetEditor;
struct iAssetEditorFactory
{

  virtual bool CanEdit(iObject *object, const AssetDescriptor &descriptor) const = 0;

  virtual iAssetEditor *CreateEditor(iObject *object, const AssetDescriptor &descriptor) const = 0;

};
#pragma once

#include <assetdescriptor.hh>
#include <cobalt/core/csclass.hh>

namespace asset::model
{
	class Asset;
}

struct iAssetEditor;
struct iAssetEditorFactory
{

  virtual bool CanEdit(iObject *object, asset::model::Asset *asset) const = 0;

  virtual iAssetEditor *CreateEditor(iObject *object, asset::model::Asset *asset) const = 0;

};
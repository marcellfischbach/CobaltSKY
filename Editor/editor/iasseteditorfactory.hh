#pragma once

#include <editor/editorexport.hh>
#include <editor/assetdescriptor.hh>
#include <csrefl/class.hh>

namespace asset::model
{
	class Asset;
}

struct iAssetEditor;
struct EDITOR_API iAssetEditorFactory
{

  virtual bool CanEdit(cs::iObject *object, asset::model::Asset *asset) const = 0;

  virtual iAssetEditor *CreateEditor(cs::iObject *object, asset::model::Asset *asset) const = 0;

};
#pragma once

#include <editor/editorexport.hh>
#include <csrefl/class.hh>
#include <editor/assetdescriptor.hh>
#include <editor/iasseteditor.refl.hh>
#include <QWidget>
#include <set>

namespace asset::model
{
	class Asset;
}

CS_CLASS()
struct EDITOR_API iAssetEditor : public cs::iObject
{
  CS_CLASS_GEN;
  iAssetEditor() : cs::iObject() { }
  virtual ~iAssetEditor() { }

  virtual void SetObject(cs::iObject *object, asset::model::Asset *asset) = 0;


  virtual QWidget *GetWidget() = 0;

  virtual const std::string &GetName() const = 0;
  virtual bool IsDirty() const = 0;

  virtual void CloseRequest () = 0;
};

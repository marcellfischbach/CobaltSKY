#pragma once

#include <cobalt/core/csclass.hh>
#include <assetdescriptor.hh>
#include <iasseteditor.refl.hh>
#include <QWidget>
#include <set>

namespace asset::model
{
	class Asset;
}

CS_INTERFACE()
struct iAssetEditor : public iObject
{
  CS_CLASS_GEN;
  iAssetEditor() : iObject() { }
  virtual ~iAssetEditor() { }

  virtual void SetObject(iObject *object, asset::model::Asset *asset) = 0;


  virtual QWidget *GetWidget() = 0;

  virtual const std::string &GetName() const = 0;
  virtual bool IsDirty() const = 0;

  virtual void CloseRequest () = 0;
};

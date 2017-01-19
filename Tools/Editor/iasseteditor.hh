#pragma once

#include <valkyrie/core/vkclass.hh>
#include <assetdescriptor.hh>
#include <iasseteditor.refl.hh>
#include <QWidget>

VK_INTERFACE()
struct iAssetEditor : public iObject
{
  VK_CLASS_GEN;
  iAssetEditor() : iObject() { }
  virtual ~iAssetEditor() { }

  virtual void SetAssetDescriptor(const AssetDescriptor &descriptor) = 0;

  virtual QWidget *GetWidget() = 0;

  virtual const vkString &GetName() const = 0;
};
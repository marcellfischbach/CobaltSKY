#pragma once

#include <iasseteditor.hh>
#include <assetdescriptor.hh>

#include <abstractasseteditor.refl.hh>

VK_INTERFACE()
class AbstractAssetEditor : public iAssetEditor
{
  VK_CLASS_GEN;
public:
  AbstractAssetEditor();
  virtual ~AbstractAssetEditor();

  virtual void SetAssetDescriptor(const AssetDescriptor &assetDescriptor);
  const AssetDescriptor &GetAssetDescriptor() const;

  void SetWidget(QWidget *widget);
  virtual QWidget *GetWidget();

  virtual const vkString &GetName() const;

private:
  AssetDescriptor m_assetDescriptor;

  QWidget *m_widget;

  vkString m_name;
};

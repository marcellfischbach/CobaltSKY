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
  virtual const std::set<vkString> &GetVisibleDockItems() const;
  virtual void PopulateDockItems();

  virtual void CloseRequest() override;

protected:
  virtual void OpenAsset() = 0;
  void AddDockItemName(const vkString &dockItemName);

private:
  AssetDescriptor m_assetDescriptor;

  QWidget *m_widget;

  bool m_dirty;
  vkString m_name;
  std::set<vkString> m_visibleDockItems;
};

#pragma once

#include <iasseteditor.hh>
#include <assetdescriptor.hh>

#include <abstractasseteditor.refl.hh>
#include <QFile>

CS_INTERFACE()
class AbstractAssetEditor : public iAssetEditor
{
  CS_CLASS_GEN;
public:
  AbstractAssetEditor();
  virtual ~AbstractAssetEditor();

  virtual void SetAssetDescriptor(const AssetDescriptor &assetDescriptor);
  const AssetDescriptor &GetAssetDescriptor() const;
  QString GetResourceFileName() const;

  void SetWidget(QWidget *widget);
  virtual QWidget *GetWidget();

  virtual const csString &GetName() const;
  virtual const std::set<csString> &GetVisibleDockItems() const;
  virtual void PopulateDockItems();

  virtual void CloseRequest() override;

protected:
  virtual void OpenAsset() = 0;
  void AddDockItemName(const csString &dockItemName);


private:
  AssetDescriptor m_assetDescriptor;

  QWidget *m_widget;

  bool m_dirty;
  csString m_name;
  std::set<csString> m_visibleDockItems;
};

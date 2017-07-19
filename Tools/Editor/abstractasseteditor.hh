#pragma once

#include <iasseteditor.hh>
#include <assetdescriptor.hh>

#include <abstractasseteditor.refl.hh>
#include <QFile>


CS_INTERFACE()
class AbstractAssetEditor : public iAssetEditor
{
  friend class AbstractAssetEditorObject;
  CS_CLASS_GEN;
public:
  AbstractAssetEditor();
  virtual ~AbstractAssetEditor();


  virtual void SetObject(iObject *object, const AssetDescriptor &assetDescriptor);
  iObject *GetEditObject();
  const iObject *GetEditObject() const;
  const AssetDescriptor &GetAssetDescriptor() const;
  QString GetResourceFileName() const;
  QString GetResourceDataFileName() const;

  void SetWidget(QWidget *widget);
  virtual QWidget *GetWidget();

  virtual const csString &GetName() const;
  virtual void SetDirty(bool dirty);
  virtual bool IsDirty() const;
  virtual const std::set<csString> &GetVisibleDockItems() const;
  virtual void PopulateDockItems();

  virtual void CloseRequest() override;
  void ResourceRenamed(const csResourceLocator &from, const csResourceLocator &to);

protected:
  virtual void UpdateAsset() = 0;
  void AddDockItemName(const csString &dockItemName);
  void UpdateMainWindow();

private:
  void UpdateName();

private:
  AssetDescriptor m_assetDescriptor;
  iObject *m_editObject;

  QWidget *m_widget;

  bool m_dirty;
  csString m_name;
  std::set<csString> m_visibleDockItems;
};

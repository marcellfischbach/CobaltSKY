#pragma once

#include <iasseteditor.hh>
#include <assetdescriptor.hh>

#include <abstractasseteditor.refl.hh>
#include <QFile>
#include <QImage>

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

  void SetWidget(QWidget *widget);
  virtual QWidget *GetWidget();

  virtual const std::string &GetName() const;
  virtual void SetDirty(bool dirty);
  virtual bool IsDirty() const;

  virtual void CloseRequest() override;
  void ResourceRenamed(const csResourceLocator &from, const csResourceLocator &to);

  void ReplacePreviewIcon(QImage image);

protected:
  virtual void UpdateAsset() = 0;
  void UpdateMainWindow();

private:
  void UpdateName();

private:
  AssetDescriptor m_assetDescriptor;
  iObject *m_editObject;

  QWidget *m_widget;

  bool m_dirty;
  std::string m_name;
  std::set<std::string> m_visibleDockItems;
};

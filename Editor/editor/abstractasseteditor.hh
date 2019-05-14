#pragma once

#include <editor/editorexport.hh>
#include <editor/iasseteditor.hh>
#include <editor/assetdescriptor.hh>
#include <editor/assetmodel/asset.hh>
#include <editor/abstractasseteditor.refl.hh>
#include <QFile>
#include <QImage>


CS_CLASS()
class EDITOR_API AbstractAssetEditor : public iAssetEditor
{
  friend class AbstractAssetEditorObject;
  CS_CLASS_GEN;
public:
  AbstractAssetEditor();
  virtual ~AbstractAssetEditor();


  virtual void SetObject(cs::iObject *object, asset::model::Asset *asset);
  cs::iObject *GetEditObject();
  const cs::iObject *GetEditObject() const;
  asset::model::Asset *GetAsset();
  const asset::model::Asset *GetAsset() const;
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
	asset::model::Asset *m_asset;
  cs::iObject *m_editObject;

  QWidget *m_widget;

  bool m_dirty;
  std::string m_name;
  std::set<std::string> m_visibleDockItems;
};

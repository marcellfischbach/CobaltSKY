#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <materialeditor/materialeditor.refl.hh>
#include <cobalt/csenums.hh>

#include <QDomDocument>
#include <QDomElement>


class QImage;
class csMaterial;
class MaterialEditorProperties;
class MaterialEditorWidget;
CS_CLASS()
class MaterialEditor : public AbstractAssetEditor
{
  CS_CLASS_GEN_OBJECT;
public:
  MaterialEditor();
  virtual ~MaterialEditor();

  void Save();
  void UpdatePreview();
  QImage TakeScreenshot(unsigned width, unsigned height);

protected:
  void UpdateAsset();
  void PopulateDockItems();

private:
  void FillElement(QDomElement materialElement, QDomDocument doc);
  void ReplaceFileContent();
  void SaveDocument(QDomDocument doc);

  csMaterial *m_material;
  MaterialEditorWidget *m_widget;

  MaterialEditorProperties *m_properties;
};


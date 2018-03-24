#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <materialeditor/materialeditor.refl.hh>
#include <cobalt/csenums.hh>

#include <QDomDocument>
#include <QDomElement>


class csfEntry;
class csfFile;
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

private:

  void FillEntry(csfEntry *materialEntry, csfFile &file);
  void ReplaceFileContent();
  void Save(csfFile &file);

  csMaterial *m_material;
  MaterialEditorWidget *m_widget;

  MaterialEditorProperties *m_properties;
};


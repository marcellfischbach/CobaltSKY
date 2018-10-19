#pragma once

#include <materialeditor/materialeditorexport.hh>
#include <editor/abstractasseteditor.hh>
#include <editor/assetdescriptor.hh>

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
class MATERIALEDITOR_API MaterialEditor : public AbstractAssetEditor
{
  CS_CLASS_GEN_OBJECT;
public:
  MaterialEditor();
  virtual ~MaterialEditor();

  void Save();
  void UpdatePreview();
  QImage TakeScreenshot(unsigned width, unsigned height);

  void MaterialAttributeChanged(const csResourceLocator &locator, const std::string &attributeID, const std::string &attributeName);

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


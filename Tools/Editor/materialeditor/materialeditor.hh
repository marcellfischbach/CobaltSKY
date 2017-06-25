#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <materialeditor/materialeditor.refl.hh>
#include <cobalt/csenums.hh>

#include <QDomDocument>
#include <QDomElement>

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

protected:
  void OpenAsset();
  void PopulateDockItems();

private:
  void FillElement(QDomElement materialElement, QDomDocument doc);
  void ReplaceFileContent();
  void SaveDocument(QDomDocument doc);

  csMaterial *m_material;
  MaterialEditorWidget *m_widget;

  MaterialEditorProperties *m_properties;
};


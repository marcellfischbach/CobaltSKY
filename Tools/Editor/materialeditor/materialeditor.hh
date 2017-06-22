#pragma once


#include <abstractasseteditor.hh>
#include <assetdescriptor.hh>

#include <materialeditor/materialeditor.refl.hh>
#include <cobalt/csenums.hh>

#include <QDomDocument>
#include <QDomElement>

class MaterialEditorWidget;
CS_CLASS()
class MaterialEditor : public AbstractAssetEditor
{
  CS_CLASS_GEN_OBJECT;
public:
  MaterialEditor();
  virtual ~MaterialEditor();

protected:
  void OpenAsset();

private:
  MaterialEditorWidget *m_widget;
};


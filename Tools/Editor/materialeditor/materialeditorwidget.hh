#pragma once


#include <QWidget>
#include <cobalt/csenums.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/csvector4f.hh>
#include <ui_materialeditorwidget.h>

class csMaterial;
class MaterialEditor;
class MaterialEditorWidget : public QWidget
{
  Q_OBJECT;
public:
  MaterialEditorWidget(MaterialEditor *editor);
  virtual ~MaterialEditorWidget();

  void SetMaterial(csMaterial *material);

private:

  MaterialEditor *m_editor;

  csMaterial *m_material;

  Ui::MaterialEditorWidget m_gui;

};
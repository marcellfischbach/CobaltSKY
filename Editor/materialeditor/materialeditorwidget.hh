#pragma once


#include <materialeditor/materialeditorexport.hh>
#include <QWidget>
#include <cobalt/csenums.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/csvector4f.hh>
#include <materialeditor/ui_materialeditorwidget.h>

class csMaterialWrapper;
class MaterialEditor;
class MATERIALEDITOR_API MaterialEditorWidget : public QWidget
{
  Q_OBJECT;
public:
  MaterialEditorWidget(MaterialEditor *editor);
  virtual ~MaterialEditorWidget();

  void SetMaterial(csMaterialWrapper *material);

  MaterialEditorPreviewSceneView *GetSceneView();

  public slots:
  void MaterialChanged();
protected slots:
  void on_pbSave_clicked();
  void on_pbScreenshot_clicked();
private:

  MaterialEditor *m_editor;

  csMaterialWrapper *m_material;

  Ui::MaterialEditorWidget m_gui;

};



#include <editors/materialeditor/materialeditorwidget.hh>
#include <editors/materialeditor/materialeditor.hh>

#include <cobalt/graphics/csmaterial.hh>


MaterialEditorWidget::MaterialEditorWidget(MaterialEditor *editor)
  : QWidget()
  , m_editor(editor)
  , m_material(0)
{
  m_gui.setupUi(this);
}

MaterialEditorWidget::~MaterialEditorWidget()
{

}

MaterialEditorPreviewSceneView *MaterialEditorWidget::GetSceneView()
{
  return m_gui.openGLWidget;
}

void MaterialEditorWidget::SetMaterial(csMaterial *material)
{
  CS_SET(m_material, material);
  m_gui.openGLWidget->SetMaterial(material);
}

void MaterialEditorWidget::MaterialChanged()
{
  m_gui.openGLWidget->repaint();
}


void MaterialEditorWidget::on_pbSave_clicked()
{
  if (m_editor)
  {
    m_editor->Save();
  }
}

void MaterialEditorWidget::on_pbScreenshot_clicked()
{
  QImage image = m_editor->TakeScreenshot(64, 64);
  m_editor->ReplacePreviewIcon(image);

}



#include <materialeditor/materialeditorwidget.hh>
#include <materialeditor/materialeditor.hh>

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


void MaterialEditorWidget::SetMaterial(csMaterial *material)
{
  CS_SET(m_material, material);
  m_gui.openGLWidget->SetMaterial(material);
}




#include <editor/editors/staticmesheditor/staticmesheditorwidget.hh>

StaticMeshEditorWidget::StaticMeshEditorWidget(StaticMeshEditor *editor)
  : QWidget()
  , m_editor(editor)
{
  m_gui.setupUi(this);
}

StaticMeshEditorWidget::~StaticMeshEditorWidget()
{

}

void StaticMeshEditorWidget::SetStaticMeshState(cs::StaticMeshState *staticMeshState)
{
  m_gui.openGLWidget->SetStaticMeshState(staticMeshState);
}

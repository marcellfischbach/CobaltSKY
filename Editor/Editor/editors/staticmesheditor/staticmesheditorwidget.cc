
#include <editors/staticmesheditor/staticmesheditorwidget.hh>

StaticMeshEditorWidget::StaticMeshEditorWidget(StaticMeshEditor *editor)
  : QWidget()
  , m_editor(editor)
{
  m_gui.setupUi(this);
}

StaticMeshEditorWidget::~StaticMeshEditorWidget()
{

}

void StaticMeshEditorWidget::SetStaticMeshState(csStaticMeshState *staticMeshState)
{
  m_gui.openGLWidget->SetStaticMeshState(staticMeshState);
}

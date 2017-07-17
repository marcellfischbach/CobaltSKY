
#include <staticmesheditor/staticmesheditorwidget.hh>

StaticMeshEditorWidget::StaticMeshEditorWidget(StaticMeshEditor *editor)
  : QWidget()
  , m_editor(editor)
{
  m_gui.setupUi(this);
}

StaticMeshEditorWidget::~StaticMeshEditorWidget()
{

}

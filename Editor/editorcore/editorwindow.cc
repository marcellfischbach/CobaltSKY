
#include <editorcore/editorwindow.hh>
#include <editorcore/itoolview.hh>


namespace cs::editor::core
{


EditorWindow::EditorWindow()
{
  setCentralWidget(new QTabWidget());
  resize(1024, 768);
}


void EditorWindow::Add(iToolView* toolView)
{
  QDockWidget* dockWidget = new QDockWidget(toolView->GetTitle(), this);
  dockWidget->setAllowedAreas(toolView->GetAllowedAreas());
  dockWidget->setWidget(toolView->GetWidget());
  addDockWidget(toolView->GetDefaultArea(), dockWidget);
}

}
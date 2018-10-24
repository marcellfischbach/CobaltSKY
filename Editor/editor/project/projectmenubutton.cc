

#include <editor/project/projectmenubutton.hh>
#include <editor/editor.hh>
#include <editor/mainwindow.hh>

ProjectMenuButton::ProjectMenuButton()
  : MainWindowMenuButton(tr("Project"), ":/icons/resources/UnknownAsset64.png")
{

}

ProjectMenuButton::~ProjectMenuButton()
{
}


ProjectMenuButton *ProjectMenuButton::Get()
{
  static ProjectMenuButton static_instance;
  return &static_instance;
}

void ProjectMenuButton::ActionPerformed()
{
  MainWindowMenuButton::ActionPerformed();
  Editor::Get()->GetMainWindow()->ShowSidePanel(new QWidget());
}
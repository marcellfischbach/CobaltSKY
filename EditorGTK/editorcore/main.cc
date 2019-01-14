
#include <gtkmm.h>
#include <editorcore/editor.hh>
#include <editorcore/mainwindow/mainwindow.hh>


int main(int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "com.cobaltsky.editor");

  editor::Editor *editor = editor::Editor::Get();
  if (!editor->Initialize(argc, argv))
  {
    return -1;
  }



  return app->run(*editor->GetMainWindow());
}

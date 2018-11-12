
#include <gtkmm.h>
#include <editor/editor.hh>
#include <editor/mainwindow.hh>


int main (int argc, char **argv)
{
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,
                                                   "com.cobaltsky.editor");

  Editor *editor = Editor::Get();

  return app->run(*editor->GetMainWindow()->GetWindow());
}

#include <editor/mainwindow.hh>
#include <gtkmm.h>


MainWindow::MainWindow()
{
  m_window = new Gtk::Window();


  m_window->set_default_size(1024, 768);
}


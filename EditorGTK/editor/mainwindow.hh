#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH


namespace Gtk
{
class Window;
}

class MainWindow
{
public:
  MainWindow();

  Gtk::Window* GetWindow ();

private:
  Gtk::Window *m_window;
};


inline Gtk::Window *MainWindow::GetWindow()
{
  return m_window;
}


#endif // MAINWINDOW_HH

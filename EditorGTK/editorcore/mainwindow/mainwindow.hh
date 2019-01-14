
#ifndef __MAINWINDOW_MAINWINDOW_HH__
#define __MAINWINDOW_MAINWINDOW_HH__

#include <gtkmm.h>

#include <map>

namespace editor
{

struct iView;

class MainWindow : public Gtk::Window
{
public:
  MainWindow ();
  virtual ~MainWindow ();

  void AddView(iView *view);

  void ShowView(iView *view);

private:
  void InitGUI();
  Gtk::ToggleButton *GetButton (iView* view);

  Gtk::Grid m_rootGrid;
  Gtk::Paned m_paned;
  Gtk::VButtonBox m_sideMenuBox;
  Gtk::Notebook m_editorsNoteBook;

  std::map<Gtk::ToggleButton*, iView*> m_menuEntries;
  iView *m_activeView;
};

}



#endif // __MAINWINDOW_MAINWINDOW_HH__

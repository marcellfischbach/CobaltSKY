
#include <editorcore/mainwindow/mainwindow.hh>
#include <editorcore/iview.hh>


namespace editor
{

MainWindow::MainWindow()
  : Gtk::Window()
  , m_activeView(nullptr)
{
  InitGUI();
  set_default_size(1024, 768);
  show_all();
}

MainWindow::~MainWindow()
{

}



void MainWindow::InitGUI()
{
  add(m_rootGrid);

  m_rootGrid.attach(m_sideMenuBox, 0, 0, 1, 1);
  m_rootGrid.attach(m_paned, 1, 0, 1, 1);

  m_sideMenuBox.set_layout(Gtk::BUTTONBOX_START);
  m_sideMenuBox.set_vexpand();
  m_sideMenuBox.set_valign(Gtk::ALIGN_FILL);
  m_sideMenuBox.set_halign(Gtk::ALIGN_FILL);

  m_paned.set_vexpand();
  m_paned.set_hexpand();
  m_paned.set_valign(Gtk::ALIGN_FILL);
  m_paned.set_halign(Gtk::ALIGN_FILL);


}

void MainWindow::AddView(iView *view)
{
  if (!view)
  {
    return;
  }

  Gtk::ToggleButton *button = new Gtk::ToggleButton(view->GetName());
  Gtk::Image* image = view->GetImage();
  if (image)
  {
    button->set_image(*image);
  }

  m_sideMenuBox.add(*button);
  button->show();
  m_menuEntries[button] = view;
}

void MainWindow::ShowView(iView *view)
{
  Gtk::ToggleButton *button = GetButton(view);
  if (!button)
  {
    return;
  }


}

Gtk::ToggleButton *MainWindow::GetButton(iView *view)
{
  for (auto entry : m_menuEntries)
  {
    if (entry.second == view)
    {
      return entry.first;
    }
  }
  return nullptr;
}


}

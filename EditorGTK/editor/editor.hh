#ifndef EDITOR_HH
#define EDITOR_HH


class MainWindow;

class Editor
{
public:
  static Editor *Get();

  MainWindow *GetMainWindow ();

private:
  Editor ();

  MainWindow *m_mainWindow;

};

inline MainWindow *Editor::GetMainWindow()
{
  return m_mainWindow;
}

#endif // EDITOR_HH

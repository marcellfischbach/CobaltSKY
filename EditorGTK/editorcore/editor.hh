#ifndef __EDITOR_HH__
#define __EDITOR_HH__

namespace editor
{

class MainWindow;

class Editor
{
public:
  static Editor *Get();
  bool Initialize (int argc, char **argv);

  MainWindow *GetMainWindow ();


private:
  Editor();


private:
  MainWindow *m_mainWindow;
};

}

#endif // __EDITOR_HH__

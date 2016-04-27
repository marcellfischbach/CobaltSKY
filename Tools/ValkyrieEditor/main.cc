
#include <stdio.h>
#include <qapplication.h>
#include <Editor.hh>
#include <master.refl.cc>

int main (int argc, char **argv)
{
  QApplication app(argc, argv);
  register_classes();

  Editor &editor = Editor::Get();
  if (!editor.Initialize(argc, argv))
  {
    return -1;
  }

  return app.exec();
}

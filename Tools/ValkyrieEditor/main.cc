
#include <stdio.h>
#include <qapplication.h>
#include <Editor.hh>

int main (int argc, char **argv)
{
  QApplication app(argc, argv);

  Editor &editor = Editor::Get();
  if (!editor.Initialize(argc, argv))
  {
    return -1;
  }

  return app.exec();
}

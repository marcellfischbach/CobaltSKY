#pragma once

#include <qobject.h>

class EditorWindow;

class Editor : public QObject
{
  Q_OBJECT
public:
  static Editor &Get();

  bool Initialize(int argc, char **argv);

private:
  Editor();

  
  EditorWindow *m_editorWindow;

};
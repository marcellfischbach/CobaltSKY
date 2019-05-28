
#pragma once

#include <editorcore/export.hh>

#include <QMainWindow>

namespace cs::editor::core
{

struct iToolView;
class Editor;

class EDITOR_CORE_API  EditorWindow : public QMainWindow
{
  friend class Editor;
public:
  void Add(iToolView* toolView);

private:
  EditorWindow();
};

}
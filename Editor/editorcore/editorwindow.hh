
#pragma once

#include <editorcore/export.hh>

#include <QMainWindow>

#include <map>

class QTabWidget;

namespace cs::editor::model
{
class AssetNode;
}

namespace cs::editor::core
{

struct iToolView;
class AbstractEditor;
class Editor;

class EDITOR_CORE_API  EditorWindow : public QMainWindow
{
  Q_OBJECT;
  friend class Editor;
public:
  void Add(iToolView* toolView);

  bool ShowOpenEditor(cs::editor::model::AssetNode* assetNode);
  bool ShowEditor(AbstractEditor* editor);

private:
  EditorWindow();

  void InitUI();

  QString GetEditorTitle(AbstractEditor* editor);

private slots:
  void OnTabCloseRequest(int idx);

private:
  std::map<cs::editor::model::AssetNode*, AbstractEditor*> m_openEditors;

  QTabWidget* m_editorTabs;
};

}
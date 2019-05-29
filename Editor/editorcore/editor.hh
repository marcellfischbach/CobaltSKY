

#pragma once

#include <editorcore/export.hh>
#include <string>
#include <vector>

namespace cs::editor::model
{
class Model;
class AssetNode;
}

namespace cs::editor::core
{

struct iEditorFactory;
class EditorWindow;

class EDITOR_CORE_API Editor
{

public:
  static Editor* Get();

  bool Initialize(int argc, char** argv);

  bool OpenProject(const std::string& projectPath);

  void OpenEditor(cs::editor::model::AssetNode* assetNode);

  void Register(iEditorFactory* editorFactory);

  EditorWindow* GetEditorWindow();

  model::Model* GetModel();

  std::string GetWindowTitle() const;

private:
  Editor();


private:
  EditorWindow* m_editorWindow;

  model::Model* m_model;

  std::vector<iEditorFactory*> m_editorFactories;

};



}
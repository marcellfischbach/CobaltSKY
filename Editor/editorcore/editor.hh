

#pragma once

#include <editorcore/export.hh>
#include <string>

namespace cs::editor::model
{
class Model;
}

namespace cs::editor::core
{

class EditorWindow;

class EDITOR_CORE_API Editor
{

public:
  static Editor* Get();

  bool Initialize(int argc, char **argv);

  bool OpenProject(const std::string& projectPath);


  EditorWindow* GetEditorWindow();

  model::Model* GetModel();

private:
  Editor();


private:
  EditorWindow* m_editorWindow;

  model::Model* m_model;

};



}
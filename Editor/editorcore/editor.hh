

#pragma once

#include <editorcore/export.hh>
#include <string>
#include <vector>

namespace cs
{
class Engine;
struct iGraphics;
class GraphicsGL4;
class ResourceLocator;
}

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

  void OpenEditor(const cs::ResourceLocator &locator);
  void OpenEditor(cs::editor::model::AssetNode* assetNode);

  void Register(iEditorFactory* editorFactory);

  EditorWindow* GetEditorWindow();

  Engine* GetEngine();

  iGraphics* GetGraphics();

  model::Model* GetModel();

  std::string GetWindowTitle() const;

private:
  Editor();


private:
  EditorWindow* m_editorWindow;

  Engine* m_engine;
  GraphicsGL4* m_graphics;

  model::Model* m_model;

  std::vector<iEditorFactory*> m_editorFactories;

};



}
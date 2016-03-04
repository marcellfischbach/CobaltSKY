#pragma once

#include <qobject.h>
#include <Valkyrie/Defs.hh>

struct IGraphics;
struct IPhysicsSystem;
class EditorWindow;
class vkEngine;


class Editor : public QObject
{
  Q_OBJECT
public:
  static Editor &Get();

  bool Initialize(int argc, char **argv);

  EditorWindow *GetEditorWindow();

  vkEngine *GetEngine();
  IGraphics *GetGraphics();
  IPhysicsSystem *GetPhysicsSystem();

private:
  Editor();

  
  EditorWindow *m_editorWindow;

  vkEngine *m_engine;
  IGraphics *m_graphics;
  IPhysicsSystem *m_physicsSystem;

};

VK_FORCEINLINE vkEngine *Editor::GetEngine()
{
  return m_engine;
}

VK_FORCEINLINE IGraphics *Editor::GetGraphics()
{
  return m_graphics;
}

VK_FORCEINLINE IPhysicsSystem *Editor::GetPhysicsSystem()
{
  return m_physicsSystem;
}


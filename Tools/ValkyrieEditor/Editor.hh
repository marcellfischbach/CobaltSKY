#pragma once

#include <qobject.h>
#include <Valkyrie/Defs.hh>
#include <QString>

class QOpenGLContext;
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

  void RequestGraphics();

  const QString &GetTitleName () const;

protected:
  void InitializeImporters();

private:
  Editor();

  
  EditorWindow *m_editorWindow;

  vkEngine *m_engine;
  IGraphics *m_graphics;
  IPhysicsSystem *m_physicsSystem;

  QString m_titleName;
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


VK_FORCEINLINE const QString &Editor::GetTitleName () const
{
  return m_titleName;
}

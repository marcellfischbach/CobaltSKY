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
  void InitializeLoaders();
  void InitializeStyleSheet();

private:
  Editor();

  
  EditorWindow *m_editorWindow;

  QString m_titleName;
};



VK_FORCEINLINE const QString &Editor::GetTitleName () const
{
  return m_titleName;
}

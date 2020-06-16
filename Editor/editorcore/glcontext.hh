

#pragma once

#include <editorcore/export.hh>

class QOffscreenSurface;
class QOpenGLContext;
class QWidget;
namespace cs::editor::core
{


class EDITOR_CORE_API GLContext
{
public:
  static GLContext& Get();
  bool MakeCurrent();

  QOpenGLContext* GetQOpenGLContext();

private:
  GLContext();
  
  QOffscreenSurface* m_surface;
  QOpenGLContext* m_context;
};

}
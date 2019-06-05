

#pragma once

#include <editorcore/export.hh>

class QOpenGLContext;

namespace cs::editor::core
{


class GLContext
{
public:
  static GLContext& Get();

  bool IsValid() const;

  QOpenGLContext* GetContext() const;

private:
  GLContext();

  QOpenGLContext* m_context;
};


}
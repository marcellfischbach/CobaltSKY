#ifndef GLCONTEXT_HH
#define GLCONTEXT_HH

#include <QObject>

class QOpenGLContext;
class QOffscreenSurface;
class GLContext
{
public:
    QOpenGLContext *GetOpenGLContext()
    {
      return m_openglContext;
    }

    void Initialize (QObject *parent);

    static GLContext *Get ();
private:
    GLContext();
    QOpenGLContext *m_openglContext = 0;
    QOffscreenSurface *m_offscreenSurface = 0;
};

#endif // GLCONTEXT_HH

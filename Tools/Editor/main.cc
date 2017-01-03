
#include <stdio.h>
#include <qapplication.h>
#include <Editor.hh>
#include <master.refl.cc>
#include <qsurfaceformat.h>


int main (int argc, char **argv)
{
  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
  QApplication app(argc, argv);
  register_classes();

  QSurfaceFormat format;
  format.setRedBufferSize(8);
  format.setGreenBufferSize(8);
  format.setBlueBufferSize(8);
  format.setAlphaBufferSize(8);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setMajorVersion(4);
  format.setMinorVersion(4);
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  //format.setOption(QSurfaceFormat::DeprecatedFunctions, true);

  //QSurfaceFormat::setDefaultFormat(format);



  Editor *editor = Editor::Get();
  if (!editor->Initialize(argc, argv))
  {
    return -1;
  }

  return app.exec();
}

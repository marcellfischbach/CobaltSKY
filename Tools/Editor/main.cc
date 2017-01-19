
#include <stdio.h>
#include <qapplication.h>
#include <editor.hh>
#include <qsurfaceformat.h>

#include <valkyrie/vkengine.hh>
#include <valkyrie/core/vksettings.hh>
#include <valkyrie/core/vkvfs.hh>


int main (int argc, char **argv)
{
  vkSettings::Initialize(argc, argv);
  vkVFS::Get()->Initialize(argc, argv);


  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
  QApplication app(argc, argv);


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

  QSurfaceFormat::setDefaultFormat(format);



  Editor *editor = Editor::Get();

  if (!editor->Initialize(argc, argv))
  {
    return -1;
  }

  return app.exec();
}

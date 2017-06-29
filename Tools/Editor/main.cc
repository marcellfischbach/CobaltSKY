
#include <stdio.h>
#include <qapplication.h>
#include <editor.hh>
#include <guitest.hh>
#include <qsurfaceformat.h>

#include <cobalt/csengine.hh>
#include <cobalt/core/cslogger.hh>
#include <cobalt/core/cssettings.hh>
#include <cobalt/core/csvfs.hh>

#include <assetmanager/assetmanagerresourcescanner.hh>

// #define GUI_TEST

int main (int argc, char **argv)
{

  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);


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
  format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  //format.setOption(QSurfaceFormat::DeprecatedFunctions, true);

  QSurfaceFormat::setDefaultFormat(format);

  QApplication app(argc, argv);

  QFile qssFile("G:/IDE/devs/CobaltSKY/Tools/Editor/cobalt.css");
  if (qssFile.open(QIODevice::ReadOnly))
  {
    QString qss = qssFile.readAll();
    qssFile.close();
    //app.setStyleSheet(qss);
  }


#ifdef GUI_TEST
  GUITest test;
  test.setVisible(true);
#else
  Editor *editor = Editor::Get();


  if (!editor->Initialize(argc, argv))
  {
    return -1;
  }
#endif
  return app.exec();
}

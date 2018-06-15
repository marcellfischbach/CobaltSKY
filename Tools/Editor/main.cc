
#include <stdio.h>
#include <qapplication.h>
#include <editor.hh>
#include <guitest.hh>
#include <qsurfaceformat.h>
#include <QStyleFactory>

#include <cobalt/csengine.hh>
#include <cobalt/core/cslogger.hh>
#include <cobalt/core/cssettings.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csfileinfo.hh>

#include <assetmanager/assetmanagerresourcescanner.hh>
#include <QIcon>

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


  QIcon icon;
  icon.addFile(":/icons/resources/CobaltSKY-Editor16.png", QSize(), QIcon::Normal, QIcon::Off);
  icon.addFile(":/icons/resources/CobaltSKY-Editor24.png", QSize(), QIcon::Normal, QIcon::Off);
  icon.addFile(":/icons/resources/CobaltSKY-Editor32.png", QSize(), QIcon::Normal, QIcon::Off);
  icon.addFile(":/icons/resources/CobaltSKY-Editor48.png", QSize(), QIcon::Normal, QIcon::Off);
  icon.addFile(":/icons/resources/CobaltSKY-Editor64.png", QSize(), QIcon::Normal, QIcon::Off);
  app.setWindowIcon(icon);

  QFile qssFile("d:/DEV/CobaltSKY/Tools/Editor/dark.qss");
	if (qssFile.open(QIODevice::ReadOnly))
  {
    QString qss = qssFile.readAll();
    qssFile.close();
    //app.setStyleSheet(qss);
  }


	if (true)
	{
		qApp->setStyle(QStyleFactory::create("Fusion"));

		QPalette darkPalette;
		darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
		darkPalette.setColor(QPalette::WindowText, Qt::white);
		darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
		darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
		darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
		darkPalette.setColor(QPalette::ToolTipText, Qt::white);
		darkPalette.setColor(QPalette::Text, Qt::white);
		darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
		darkPalette.setColor(QPalette::ButtonText, Qt::white);
		darkPalette.setColor(QPalette::BrightText, Qt::red);
		darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

		darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
		darkPalette.setColor(QPalette::HighlightedText, Qt::black);

		//qApp->setPalette(darkPalette);
		//qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
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

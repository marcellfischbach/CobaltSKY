
#include <stdio.h>
#include <qapplication.h>
#include <editor/editor.hh>
#include <editor/guitest.hh>
#include <qsurfaceformat.h>
#include <QStyleFactory>

#include <cobalt/csengine.hh>
#include <cobalt/core/cslogger.hh>
#include <cobalt/core/cssettings.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csresourcelocator.hh>

#include <editor/assetmanager/assetmanagerresourcescanner.hh>

#include <imageimporter/imageimportermodule.hh>

#include <materialeditor/materialeditormodule.hh>
#include <samplereditor/samplereditormodule.hh>
#include <shadergrapheditor/shadergrapheditormodule.hh>
#include <textureeditor/textureeditormodule.hh>
#include <QIcon>

#include <cobalt/graphics/itexture.hh>

// #define GUI_TEST


int main (int argc, char **argv)
{
  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

  QApplication app(argc, argv);


  QIcon icon;
  icon.addFile(":/icons/resources/CobaltSKY-Editor16.png", QSize(), QIcon::Normal, QIcon::Off);
  icon.addFile(":/icons/resources/CobaltSKY-Editor24.png", QSize(), QIcon::Normal, QIcon::Off);
  icon.addFile(":/icons/resources/CobaltSKY-Editor32.png", QSize(), QIcon::Normal, QIcon::Off);
  icon.addFile(":/icons/resources/CobaltSKY-Editor48.png", QSize(), QIcon::Normal, QIcon::Off);
  icon.addFile(":/icons/resources/CobaltSKY-Editor64.png", QSize(), QIcon::Normal, QIcon::Off);
  app.setWindowIcon(icon);

  QFile qssFile("d:/DEV/CobaltSKY/Editor/editor/dark_orange.qss");
  if (false && qssFile.open(QIODevice::ReadOnly))
  {
    QString qss = qssFile.readAll();
    qssFile.close();
    app.setStyleSheet(qss);
  }

  if (true)
  {
    printf("Styles:\n");
    QStringList styles = QStyleFactory::keys();
    for (QString style : styles)
    {
      printf("  %s\n", style.toLatin1().data());
    }

    qApp->setStyle(QStyleFactory::create("Adwaita"));
    if (false)
    {
      QPalette darkPalette;
      darkPalette.setColor(QPalette::Window, QColor(53, 53, 63));
      darkPalette.setColor(QPalette::WindowText, Qt::white);
      darkPalette.setColor(QPalette::Base, QColor(25, 25, 35));
      darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 63));
      darkPalette.setColor(QPalette::AlternateBase, QColor(40, 40, 50));
      darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
      darkPalette.setColor(QPalette::ToolTipText, Qt::white);
      darkPalette.setColor(QPalette::Text, Qt::white);
      darkPalette.setColor(QPalette::Button, QColor(53, 53, 63));
      darkPalette.setColor(QPalette::ButtonText, Qt::white);
      darkPalette.setColor(QPalette::BrightText, Qt::red);
      darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
      darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
      darkPalette.setColor(QPalette::HighlightedText, Qt::black);

      darkPalette.setColor(QPalette::Disabled, QPalette::Text, Qt::gray);
      darkPalette.setColor(QPalette::Disabled, QPalette::Button, QColor(25, 25, 35));
      darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::gray);

      qApp->setPalette(darkPalette);
      qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

    }
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

  ImageImporterModule::Initialize(argc, argv);
  MaterialEditorModule::Initialize(argc, argv);
  SamplerEditorModule::Initialize(argc, argv);
  ShaderGraphEditorModule::Initialize(argc, argv);
  TextureEditorModule::Initialize(argc, argv);
  
#endif
  return app.exec();
}

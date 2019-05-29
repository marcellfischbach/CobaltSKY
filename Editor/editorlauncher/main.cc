
#include <qapplication.h>
#include <editorcore/editor.hh>
#include <assetmanager/assetmanagermodule.hh>
#include <pluginsamplereditor/samplereditormodule.hh>
#include <QIcon>
#include <QPalette>
#include <QStyleFactory>
#include <filesystem>

// #define GUI_TEST



void cleanup()
{
  try
  {
    std::filesystem::path path("d:/DEV/temp/CobaltSKYModelTest-Ref");
    std::filesystem::remove_all(path);
  }
  catch (const std::exception & e)
  {
    printf("Unable to clean up: %s\n", e.what());
  }

}

void copy_test_folder()
{
  try
  {
    std::filesystem::path src("d:/DEV/temp/CobaltSKYModelTest");
    std::filesystem::path dst("d:/DEV/temp/CobaltSKYModelTest-Ref");
    std::filesystem::copy(src, dst, std::filesystem::copy_options::recursive);
  }
  catch (const std::exception & e)
  {
    printf("Unable to copy test data: %s\n", e.what());
  }
}


int main (int argc, char **argv)
{
  cleanup();
  copy_test_folder();


  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

  QApplication app(argc, argv);
  qApp->setStyle(QStyleFactory::create("Fusion"));
  if (true)
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

  QIcon icon;
  icon.addFile(":/icons/cs_16");
  icon.addFile(":/icons/cs_24");
  icon.addFile(":/icons/cs_32");
  icon.addFile(":/icons/cs_48");
  icon.addFile(":/icons/cs_64");
  app.setWindowIcon(icon);







  cs::editor::core::Editor *editor = cs::editor::core::Editor::Get();


  if (!editor->Initialize(argc, argv))
  {
    return -1;
  }

  cs::editor::assetmanager::Module::Initialize(argc, argv);
  cs::editor::plugin::samplereditor::Module::Initialize(argc, argv);
  /*
  ImageImporterModule::Initialize(argc, argv);
  MaterialEditorModule::Initialize(argc, argv);
  SamplerEditorModule::Initialize(argc, argv);
  ShaderGraphEditorModule::Initialize(argc, argv);
  TextureEditorModule::Initialize(argc, argv);
  */


  return app.exec();
}

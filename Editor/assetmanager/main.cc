
#include <assetmanager/tree/assetmanagertreeview.hh>
#include <assetmanager/tree/assetmanagertreemodel.hh>

#include <cobalt/core/cssettings.hh>
#include <cobalt/core/csvfs.hh>

#include <editormodel/model.hh>
#include <editormodel/pathscanner.hh>

#include <iostream>
#include <filesystem>

#include <QApplication>
#include <QMainWindow>
#include <QStyleFactory>

using namespace cs::editor;




void cleanup()
{
  try
  {
    std::filesystem::path path("d:/DEV/temp/CobaltSKYModelTest-Ref");
    std::filesystem::remove_all(path);
  }
  catch (const std::exception &e)
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
  catch (const std::exception &e)
  {
    printf("Unable to copy test data: %s\n", e.what());
  }
}


class TestWindow : public QMainWindow
{
public:
  TestWindow(model::Model *model)
    : QMainWindow()
  {
    assetmanager::TreeView *treeView = new assetmanager::TreeView(model, this);

    setCentralWidget(treeView);
  }
};

int main(int argc, char **argv)
{

  cleanup();
  copy_test_folder();

  csSettings::Get()->Initialize("d:/DEV/temp/CobaltSKYModelTest-Ref/config.csf");
  csVFS::Get()->Initialize(csSettings::Get());

  csVFS* vfs = csVFS::Get();

  cs::editor::model::Model model;
  cs::editor::model::PathScanner ps(&model);



  for (size_t i = 0, in = csVFS::Get()->GetNumberOfEntries(); i < in; ++i)
  {
    cs::editor::model::VFSEntryNode *entryNode = model.CreateVFSEntryNode();
    entryNode->SetEntry(&csVFS::Get()->GetEntry(i));

    cs::editor::model::Transaction tx = model.CreateTransaction();
    tx.Begin();
    model.Add(entryNode, model.GetRoot(), tx);
    ps.Scan(entryNode, tx);
    tx.Commit();

  }


  model.Debug();




  QApplication app(argc, argv);
  qApp->setStyle(QStyleFactory::create("Adwaita"));


  TestWindow window(&model);
  window.resize(256, 768);
  window.setVisible(true);

  return app.exec();
}
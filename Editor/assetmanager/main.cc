
#include <assetmanager/assetmanager.hh>
#include <assetmanager/menu/menuitemfactories.hh>
#include <assetmanager/menu/items/delete/deletemenuitem.hh>
#include <assetmanager/menu/items/newfolder/newfoldermenuitem.hh>
#include <assetmanager/menu/items/rename/renamemenuitem.hh>

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
  TestWindow(model::Model *model, assetmanager::AssetManager::Style style)
    : QMainWindow()
  {
    assetmanager::AssetManager *assetManager = new assetmanager::AssetManager(model, style, this);

    setCentralWidget(assetManager);
  }
};

int main(int argc, char **argv)
{

  cleanup();
  copy_test_folder();

  cs::Settings::Get()->Initialize("d:/DEV/temp/CobaltSKYModelTest-Ref/config.csf");
  cs::VFS::Get()->Initialize(cs::Settings::Get());

  cs::VFS* vfs = cs::VFS::Get();

  cs::editor::model::Model model;
  cs::editor::model::PathScanner ps(&model);



  for (size_t i = 0, in = cs::VFS::Get()->GetNumberOfEntries(); i < in; ++i)
  {
    cs::editor::model::VFSEntryNode *entryNode = model.CreateVFSEntryNode();
    entryNode->SetEntry(&cs::VFS::Get()->GetEntry(i));

    cs::editor::model::Transaction tx = model.CreateTransaction();
    tx.Begin();
    model.Add(entryNode, model.GetRoot(), tx);
    ps.Scan(entryNode, tx);
    tx.Commit();

  }


  model.Debug();

  if (false)
  {
    model.OnTreeStructNodeAdded().Connect([](cs::editor::model::Node* child, cs::editor::model::Node* parent)
    {
      printf("TreeStructNodeAdded: %s -> %s\n", child->GetResourceLocator().Encode().c_str(), parent->GetResourceLocator().Encode().c_str());
    }
    );

    model.OnTreeStructNodeMoved().Connect([](cs::editor::model::Node *node, cs::editor::model::Node* oldParent, cs::editor::model::Node* newParent)
    {
      printf("TreeStructNodeMoved: %s: %s -> %s\n", node->GetName().c_str(), oldParent->GetResourceLocator().Encode().c_str(), newParent->GetResourceLocator().Encode().c_str());
    }
    );
    model.OnTreeStructNodeChanged().Connect([](cs::editor::model::Node* node)
    {
      printf("TreeStructNodeChanged: %s: %s\n", node->GetName().c_str(), node->GetResourceLocator().Encode().c_str());
    }
    );
    model.OnTreeStructNodeRemoved().Connect([](cs::editor::model::Node* child, cs::editor::model::Node* parent)
    {
      printf("TreeStructNodeRemoved: %s <> %s\n", child->GetResourceLocator().Encode().c_str(), parent->GetResourceLocator().Encode().c_str());
    }
    );

    model.OnNamedNodeAdded().Connect([](cs::editor::model::Node* child, cs::ResourceLocator locator)
    {
      printf("NamedNodeAdded: %s: %s\n", child->GetName().c_str(), locator.Encode().c_str());
    }
    );

    model.OnNamedNodeRenamed().Connect([](cs::editor::model::Node* child, cs::ResourceLocator oldLocator, cs::ResourceLocator newLocator)
    {
      printf("NamedNodeRenamed: %s: %s -> %s\n", child->GetName().c_str(), oldLocator.Encode().c_str(), newLocator.Encode().c_str());
    }
    );
    model.OnNamedNodeRemoved().Connect([](cs::editor::model::Node* child, cs::ResourceLocator locator)
    {
      printf("NamedNodeRemoved: %s: %s\n", child->GetName().c_str(), locator.Encode().c_str());
    }
    );


    model.OnResourceNameAdded().Connect([](cs::ResourceLocator locator)
    {
      printf("ResourceNameAdded: %s\n", locator.Encode().c_str());
    }
    );
    model.OnResourceNameChanged().Connect([](cs::ResourceLocator locator)
    {
      printf("ResourceNameChanged: %s\n", locator.Encode().c_str());
    }
    );
    model.OnResourceNameRenamed().Connect([](cs::ResourceLocator oldLocator, cs::ResourceLocator newLocator)
    {
      printf("ResourceNameRenamed: %s -> %s\n", oldLocator.Encode().c_str(), newLocator.Encode().c_str());
    }
    );
    model.OnResourceNameRemoved().Connect([](cs::ResourceLocator locator)
    {
      printf("ResourceNameRemoved: %s\n", locator.Encode().c_str());
    }
    );

  }

  cs::editor::assetmanager::MenuItemFactories &factories = cs::editor::assetmanager::MenuItemFactories::Get();
  factories.AddFactory(new cs::editor::assetmanager::NewFolderMenuItemFactory());
  factories.AddFactory(new cs::editor::assetmanager::DeleteMenuItemFactory());
  factories.AddFactory(new cs::editor::assetmanager::RenameMenuItemFactory());

  QApplication app(argc, argv);

  QIcon icon;
  icon.addFile(":/icons/cs_16");
  icon.addFile(":/icons/cs_24");
  icon.addFile(":/icons/cs_32");
  icon.addFile(":/icons/cs_48");
  icon.addFile(":/icons/cs_64");
  app.setWindowIcon(icon);

  qApp->setStyle(QStyleFactory::create("Adwaita"));
  //qApp->setStyle(QStyleFactory::create("Fusion"));

  /*
  TestWindow windowTree(&model, cs::editor::assetmanager::AssetManager::eS_Automatic);
  windowTree.resize(256, 768);
  windowTree.move(100, 50);
  windowTree.setVisible(true);
  */


  TestWindow windowList(&model, cs::editor::assetmanager::AssetManager::eS_Automatic);
  windowList.resize(1024, 256);
  windowList.move(100, 50 + 768 + 32);
  windowList.setVisible(true);

  return app.exec();
}
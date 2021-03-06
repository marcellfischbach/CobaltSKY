

#include <iostream>
#include <editormodel/model.hh>
#include <editormodel/pathscanner.hh>
#include <cobalt/core/cssettings.hh>
#include <cobalt/core/csvfs.hh>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <cstdlib>



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


void test_delete(cs::editor::model::Model &model)
{
  cs::editor::model::Node *node;

  cs::editor::model::Transaction tx = model.CreateTransaction();

  try
  {
    tx.Begin();
  }
  catch (const std::exception &e)
  {
    printf("Unable to begin transation: %s\n", e.what());
    return;
  }

  std::string locatorName = "Engine@models/";
  try
  {
    node = model.FindNode(cs::ResourceLocator(locatorName));
    if (node)
    {
      model.Delete(node, tx);
    }
    else
    {
      printf("Unable to delete node: %s\n", locatorName.c_str());
    }

    tx.Commit();
  }
  catch (const cs::editor::model::FSDirectoryNotEmptyException &ne)
  {
    tx.Rollback();

    std::cout << "DirectoryNotEmpty: " << ne.what() << std::endl;
  }
  catch (const std::exception &e)
  {
    tx.Rollback();
    printf("Unable to comply: %s\n", e.what());
  }

}
void test_rename(cs::editor::model::Model &model)
{
  cs::editor::model::Node *node;

  cs::editor::model::Transaction tx = model.CreateTransaction();

  try
  {
    tx.Begin();
  }
  catch (const std::exception &e)
  {
    printf("Unable to begin transation: %s\n", e.what());
    return;
  }

  std::string locatorName = "Engine@models/";
  try
  {
    node = model.FindNode(cs::ResourceLocator(locatorName));
    if (node)
    {
      model.Rename(node, "other_models", tx);
    }
    else
    {
      printf("Unable to find node: %s\n", locatorName.c_str());
    }

    tx.Commit();
  }
  catch (const std::exception &e)
  {
    tx.Rollback();
    printf("Unable to comply: %s\n", e.what());
  }

}

void test_move(cs::editor::model::Model &model)
{
  cs::editor::model::Node *sourceNode, *destinatioNode;

  cs::editor::model::Transaction tx = model.CreateTransaction();

  try
  {
    tx.Begin();
  }
  catch (const std::exception &e)
  {
    printf("Unable to begin transation: %s\n", e.what());
    return;
  }

  std::string sourceLocatorName = "Engine@models/";
  std::string destinationLocatorName = "Engine@materials/";
  try
  {
    sourceNode = model.FindNode(cs::ResourceLocator(sourceLocatorName));
    destinatioNode = model.FindNode(cs::ResourceLocator(destinationLocatorName));
    if (sourceNode && destinatioNode)
    {
      model.Move(sourceNode, destinatioNode->AsFolderNode(), tx);
    }
    else
    {
      printf("Unable to find node: %s\n", sourceLocatorName.c_str());
    }

    tx.Commit();
  }
  catch (const std::exception &e)
  {
    tx.Rollback();
    printf("Unable to comply: %s\n", e.what());
  }

}




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


  //test_rename(model);
  //test_delete(model);
  test_move(model);

  std::cin.get();
  return 0;
}

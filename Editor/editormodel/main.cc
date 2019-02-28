

#include <iostream>
#include <editormodel/model.hh>
#include <editormodel/pathscanner.hh>
#include <cobalt/core/cssettings.hh>
#include <cobalt/core/csvfs.hh>
#include <filesystem>


void cleanup()
{
  std::filesystem::path path("d:/DEV/temp/CobaltSKYModelTest-Ref");
  std::filesystem::remove_all(path);

}

void copy_test_folder()
{
  std::filesystem::path src("d:/DEV/temp/CobaltSKYModelTest");
  std::filesystem::path dst("d:/DEV/temp/CobaltSKYModelTest-Ref");
  std::filesystem::copy(src, dst, std::filesystem::copy_options::recursive);
}


void test_rename(cs::editor::model::Model &model)
{
  cs::editor::model::Node *node;

  cs::editor::model::Transaction tx;


  tx.Begin();
  try
  {
    node = model.FindNode(csResourceLocator("Engine@models/"));
    if (node)
    {
      model.OnResourceNameRenamed().Connect([&model, node](csResourceLocator oldLocator, csResourceLocator newLocator)
      {
        printf("ResourceNameRenamed: %s -> %s\n", oldLocator.Encode().c_str(), newLocator.Encode().c_str());
        node->Debug(2);
      }
      );

      model.Rename(node, "other_models", tx);
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


  csSettings::Get()->Initialize("d:/DEV/temp/CobaltSKYModelTest-Ref/config.csf");
  csVFS::Get()->Initialize(csSettings::Get());

  csVFS* vfs = csVFS::Get();

  cs::editor::model::Model model;
  cs::editor::model::PathScanner ps(&model);



  for (size_t i = 0, in = csVFS::Get()->GetNumberOfEntries(); i < in; ++i)
  {
    cs::editor::model::VFSEntryNode *entryNode = model.CreateVFSEntryNode();
    entryNode->SetEntry(&csVFS::Get()->GetEntry(i));

    cs::editor::model::Transaction tx;
    tx.Begin();
    model.Add(entryNode, model.GetRoot(), tx);
    ps.Scan(entryNode, tx);
    tx.Commit();

  }


  model.Debug();

  /*
  model.OnTreeStructNodeAdded().Connect([](cs::editor::model::Node* child, cs::editor::model::Node* parent)
  {
    printf("TreeStructNodeAdded: %s -> %s\n", child->GetResourceLocator().Encode().c_str(), parent->GetResourceLocator().Encode().c_str());
  }
  );

  model.OnNamedNodeNameAdded().Connect([](cs::editor::model::Node* child, csResourceLocator locator)
  {
    printf("NamedNodeNameAdded: %s: %s\n", child->GetResourceLocator().Encode().c_str(), locator.Encode().c_str());
  }
  );

  model.OnNamedNodeNameChanged().Connect([](cs::editor::model::Node* child, csResourceLocator oldLocator, csResourceLocator newLocator)
  {
    printf("NamedNodeNameChanged: %s: %s -> %s\n", child->GetResourceLocator().Encode().c_str(), oldLocator.Encode().c_str(), newLocator.Encode().c_str());
  }
  );

  model.OnResourceNameAdded().Connect([](csResourceLocator locator) 
  {
    printf("ResourceNameAdded: %s\n", locator.Encode().c_str());
  }
  );
  model.OnResourceNameRenamed().Connect([](csResourceLocator oldLocator, csResourceLocator newLocator)
  {
    printf("ResourceNameRenamed: %s -> %s\n", oldLocator.Encode().c_str(), newLocator.Encode().c_str());
  }
  );
  model.OnResourceNameRemoved().Connect([](csResourceLocator locator)
  {
    printf("ResourceNameRemoved: %s\n", locator.Encode().c_str());
  }
  );
  */


  test_rename(model);

  std::cin.get();
  return 0;
}


#include <editormodel/pathscanner.hh>
#include <editormodel/model.hh>
#include <editormodel/nodescanner.hh>
#include <cobalt/core/csvfs.hh>
#include <filesystem>

namespace std
{
namespace fs = filesystem;
}

namespace cs::editor::model
{

PathScanner::PathScanner(Model *model)
  : m_model(model)
{
	std::string narf = 'Das ist ein \"String\"\n';
}


void PathScanner::Scan(VFSEntryNode *entry, Transaction &tx)
{
  if (!entry)
  {
    return;
  }

  std::string path = csVFS::Get()->GetAbsolutePath(entry->GetResourceLocator(), csVFS::CheckExistence);
  if (path.empty())
  {
    return;
  }

  // printf("Scan: '%s' -> '%s'\n", entry->GetEntry()->GetName().c_str(), entry->GetResourceLocator().Encode().c_str());
  ScanFolder(entry, std::fs::path(path), tx);
}


void PathScanner::ScanFolder(FolderNode *parentNode, const std::fs::path &path, Transaction &tx)
{

  for (const std::fs::path &path : std::fs::directory_iterator(path))
  {
    Scan(parentNode, path, tx);
  }
}

void PathScanner::Scan(FolderNode *parentNode, const std::fs::path &path, Transaction &tx)
{
  Model *model = parentNode->GetModel();
  //printf("   %s\n", path.generic_string().c_str());
  if (std::fs::is_regular_file(path))
  {
    if (path.extension() == ".csf" || path.extension() == ".asset")
    {
      AssetNode *assetNode = model->CreateAssetNode();
      assetNode->SetName(path.filename().generic_string());
      model->Add(assetNode, parentNode, tx);
      NodeScanner nodeScanner;
      nodeScanner.Scan(assetNode);
    }
  }
  else if (std::fs::is_directory(path))
  {
    //printf("  Directory: %s\n", path.filename().generic_string().c_str());

    FolderNode *folderNode = model->CreateFolderNode();
    folderNode->SetName(path.filename().generic_string());
    model->Add(folderNode, parentNode, tx);
    ScanFolder(folderNode, path, tx);
  }

}


}

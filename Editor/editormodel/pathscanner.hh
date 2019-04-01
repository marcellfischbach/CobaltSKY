
#pragma once

#include <editormodel/editormodelexport.hh>
#include <string>

namespace std::filesystem
{
class path;
}

namespace cs::editor::model
{

// comment here	
class FolderNode;
class Model;
class VFSEntryNode;
class Transaction;
	
	

class  EDITOR_MODEL_API  PathScanner
{

public:
	/* comment
    befor constructor */
// line comment
  PathScanner(Model *model,char * test = "hel\"//*lo");

  void Scan(VFSEntryNode *vfsEntry, Transaction &tx);

private:
  void Scan(FolderNode *parent, const std::filesystem::path &path, Transaction &tx);
  void ScanFolder(FolderNode *parent, const std::filesystem::path &path, Transaction &tx);

private:
  Model *m_model;
};


}
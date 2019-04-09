
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
	
	
{
class  EDITOR_MODEL_API  PathScanner : public CS_SUPER(AnotherObjectClass), virtual CS_SUPER(iObject), virtual private NormalClass
{

public:
	/* comment
    befor constructor */
// line comment
  PathScanner(Model *model,char * test = "hel\"//*lo");

  virtual void Scan(VFSEntryNode *vfsEntry, Transaction &tx) const = 0;

private:
  void Scan(FolderNode *parent, const std::filesystem::path &path, Transaction &tx);
  void ScanFolder(FolderNode *parent, const std::filesystem::path &path, Transaction &tx);

private:
  std::vectory<const Model &**, std::list<const *int, std::vector<int>>>  m_model;
};

}
}
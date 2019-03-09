
#pragma once

#include <editormodel/editormodelexport.hh>
#include <set>
#include <string>

class csfEntry;
class csResourceLocator;

namespace cs::editor::model
{


class AssetNode;
class EDITOR_MODEL_API NodeScanner
{
public:
  NodeScanner();
  void Scan(AssetNode *asset);


  std::string GetTypeName() const;
  std::set<csResourceLocator> GetReferenceLocators() const;

private:
  void ScanTypeName(const csfEntry *rootEntry);
  void Scan(const csfEntry *entry);

  std::string m_typeName;
  std::set<std::string> m_references;

};


}
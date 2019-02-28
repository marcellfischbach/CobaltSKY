
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


  std::set<csResourceLocator> GetReferenceLocators() const;

private:
  void Scan(const csfEntry *entry);

  std::set<std::string> m_references;

};


}
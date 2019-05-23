
#pragma once

#include <editormodel/editormodelexport.hh>
#include <set>
#include <string>

namespace cs
{
class ResourceLocator;
}

namespace cs::file
{
class Entry;

}

namespace cs::editor::model
{


class AssetNode;
class EDITOR_MODEL_API NodeScanner
{
public:
  NodeScanner();
  void Scan(AssetNode *asset);


  std::string GetTypeName() const;
  std::set<cs::ResourceLocator> GetReferenceLocators() const;

private:
  void ScanTypeName(const cs::file::Entry *rootEntry);
  void Scan(const cs::file::Entry *entry);

  std::string m_typeName;
  std::set<std::string> m_references;

};


}
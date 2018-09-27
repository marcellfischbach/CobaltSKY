#pragma once

#include <Editor/editorexport.hh>
#include <vector>

namespace asset::model
{
  class Asset;
}

struct iObject;
struct iAssetImporterFactory;
class EDITOR_API ImporterFactoryManager
{

public:
  static ImporterFactoryManager *Get();

  void AddImporterFactory(iAssetImporterFactory *factory);
  std::vector<iAssetImporterFactory*> GetFactories() const;


private:
  ImporterFactoryManager();

  std::vector<iAssetImporterFactory*> m_factories;
};


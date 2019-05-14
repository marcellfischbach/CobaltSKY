#pragma once

#include <Editor/editorexport.hh>
#include <vector>

namespace asset::model
{
  class Asset;
}

struct cs::iObject;
struct iAssetEditorFactory;
class EDITOR_API EditorFactoryManager
{

public:
  static EditorFactoryManager *Get();

  void AddEditorFactory(iAssetEditorFactory *factory);
  iAssetEditorFactory *FindFactory(cs::iObject *object, asset::model::Asset *asset);


private:
  EditorFactoryManager();

  std::vector<iAssetEditorFactory*> m_factories;
};
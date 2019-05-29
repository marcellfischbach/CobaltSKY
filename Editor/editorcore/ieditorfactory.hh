

#pragma once

namespace cs::editor::model
{
class AssetNode;
}

namespace cs::editor::core
{

class AbstractEditor;
struct iEditorFactory
{

  virtual ~iEditorFactory() { }


  virtual bool CanEdit(cs::editor::model::AssetNode* assetNode) = 0;

  virtual AbstractEditor* CreateEditor(cs::editor::model::AssetNode* assetNode) = 0;


};

}
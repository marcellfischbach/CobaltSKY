

#pragma once

#include <editorcore/ieditorfactory.hh>

namespace cs::editor::plugin::textureeditor
{


class TextureEditorFactory : public cs::editor::core::iEditorFactory
{
public:
  virtual ~TextureEditorFactory()  { }

  virtual bool CanEdit(cs::editor::model::AssetNode* assetNode);

  virtual cs::editor::core::AbstractEditor* CreateEditor(cs::editor::model::AssetNode* assetNode) ;

};

}
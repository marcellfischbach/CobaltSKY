

#pragma once

#include <editorcore/ieditorfactory.hh>

namespace cs::editor::plugin::samplereditor
{


class SamplerEditorFactory : public cs::editor::core::iEditorFactory
{
public:
  virtual ~SamplerEditorFactory()  { }

  virtual bool CanEdit(cs::editor::model::AssetNode* assetNode);

  virtual cs::editor::core::AbstractEditor* CreateEditor(cs::editor::model::AssetNode* assetNode) ;

};

}
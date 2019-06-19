
#pragma once

#include <editorcore/abstracteditor.hh>

namespace cs
{
struct iTexture;
}

namespace cs::editor::plugin::textureeditor
{


class TextureEditor : public cs::editor::core::AbstractEditor
{
public:
  TextureEditor(cs::editor::model::AssetNode *assetNode, iTexture *sampler);

};
}
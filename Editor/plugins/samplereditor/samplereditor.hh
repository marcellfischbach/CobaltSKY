
#pragma once

#include <editorcore/abstracteditor.hh>

namespace cs
{
struct iSampler;
}

namespace cs::editor::plugin::samplereditor
{


class SamplerEditor : public cs::editor::core::AbstractEditor
{
public:
  SamplerEditor(cs::editor::model::AssetNode *assetNode, iSampler *sampler);

};
}
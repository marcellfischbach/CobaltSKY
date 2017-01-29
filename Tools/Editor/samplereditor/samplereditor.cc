
#include <samplereditor/samplereditor.hh>
#include <samplereditor/samplereditorwidget.hh>

#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/graphics/isampler.hh>

SamplerEditor::SamplerEditor()
  : AbstractAssetEditor()
{
  VK_CLASS_GEN_CONSTR;
  m_widget = new SamplerEditorWidget(this);
  SetWidget(m_widget);
}

SamplerEditor::~SamplerEditor()
{
}


void SamplerEditor::OpenAsset()
{
  const AssetDescriptor &descriptor = GetAssetDescriptor();
  

  iSampler *sampler = vkResourceManager::Get()->Aquire<iSampler>(vkResourceLocator(descriptor.GetAssetResourceName()));
  m_widget->SetSampler(sampler);
}

void SamplerEditor::Apply()
{
  m_widget->Apply();
}

void SamplerEditor::Save()
{
  m_widget->Apply();
  m_widget->Store();
}

void SamplerEditor::Reset()
{
  m_widget->Reset();
  m_widget->Apply();
}

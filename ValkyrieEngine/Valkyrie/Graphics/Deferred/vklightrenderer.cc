
#include <Valkyrie/Graphics/Deferred/vklightrenderer.hh>
#include <Valkyrie/Graphics/Deferred/vkdefaultcollector.hh>
#include <Valkyrie/Graphics/Deferred/vkgbuffer.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/entity/vkentity.hh>
#include <Valkyrie/entity/vkgeometrydata.hh>
#include <Valkyrie/entity/vkrenderstate.hh>
#include <Valkyrie/Graphics/Camera.hh>
#include <Valkyrie/Graphics/Light.hh>
#include <Valkyrie/Math/Clipper.hh>


vkLightRenderer::vkLightRenderer(IGraphics *renderer)
  : m_renderer(renderer)
{
  m_depthSampler = m_renderer->CreateSampler();
  m_depthSampler->SetFilter(eFM_MinMagLinear);
  m_depthSampler->SetAddressU(eTAM_ClampBorder);
  m_depthSampler->SetAddressV(eTAM_ClampBorder);
  m_depthSampler->SetAddressW(eTAM_ClampBorder);
  m_depthSampler->SetTextureCompareMode(eTCM_CompareToR);
  m_depthSampler->SetTextureCompareFunc(eTCF_Less);
}


vkLightRenderer::~vkLightRenderer()
{
  VK_RELEASE(m_depthSampler);
  VK_RELEASE(m_shadowBuffer);
}


void vkLightRenderer::InitializeLightProgram(LightProgram *lightProgram, const vkResourceLocator &locator)
{
  lightProgram->program = vkResourceManager::Get()->GetOrLoad<IShader>(locator);
  if (lightProgram->program)
  {
    lightProgram->gbuffer.attrDiffuseRoughness = lightProgram->program->GetAttribute(vkShaderAttributeID("DiffuseRoughness"));
    lightProgram->gbuffer.attrNormalLightMode = lightProgram->program->GetAttribute(vkShaderAttributeID("NormalLightMode"));
    lightProgram->gbuffer.attrEmissiveMetallic = lightProgram->program->GetAttribute(vkShaderAttributeID("EmissiveMetallic"));
    lightProgram->gbuffer.attrSSSSpecular = lightProgram->program->GetAttribute(vkShaderAttributeID("SSSSpecular"));
    lightProgram->gbuffer.attrDepth = lightProgram->program->GetAttribute(vkShaderAttributeID("Depth"));

    lightProgram->attrColor = lightProgram->program->GetAttribute(vkShaderAttributeID("LightColor"));
    lightProgram->attrEnergy = lightProgram->program->GetAttribute(vkShaderAttributeID("LightEnergy"));
  }
}


void vkLightRenderer::BindGBuffer(GBufferAttribs &attribs, vkGBuffer *gbuffer)
{
  if (attribs.attrDiffuseRoughness)
  {
    vkTextureUnit unit = m_renderer->BindTexture(gbuffer->GetDiffuseRoughness());
    attribs.attrDiffuseRoughness->Set(unit);
  }
  if (attribs.attrNormalLightMode)
  {
    vkTextureUnit unit = m_renderer->BindTexture(gbuffer->GetNormalLightMode());
    attribs.attrNormalLightMode->Set(unit);
  }
  if (attribs.attrEmissiveMetallic)
  {
    vkTextureUnit unit = m_renderer->BindTexture(gbuffer->GetEmissiveMetallic());
    attribs.attrEmissiveMetallic->Set(unit);
  }
  if (attribs.attrSSSSpecular)
  {
    vkTextureUnit unit = m_renderer->BindTexture(gbuffer->GetSSSSpec());
    attribs.attrSSSSpecular->Set(unit);
  }
  if (attribs.attrDepth)
  {
    vkTextureUnit unit = m_renderer->BindTexture(gbuffer->GetDepth());
    attribs.attrDepth->Set(unit);
  }
}

void vkLightRenderer::BindLight(LightProgram &lightProgram, vkLight *light)
{
  if (lightProgram.attrColor)
  {
    lightProgram.attrColor->Set(light->GetColor());
    VK_CHECK_GRAPHICS_ERROR(m_renderer);
  }
  if (lightProgram.attrEnergy)
  {
    lightProgram.attrEnergy->Set(light->GetEnergy());
    VK_CHECK_GRAPHICS_ERROR(m_renderer);
  }
}



void vkLightRenderer::CalcShadowIntensity(const vkLight *light)
{
  float shadowIntensity = light->GetShadowIntensity();
  m_shadowIntensity.Set(1.0f - shadowIntensity, shadowIntensity);
}





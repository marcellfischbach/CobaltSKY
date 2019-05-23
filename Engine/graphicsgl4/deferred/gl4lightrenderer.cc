
#include <graphicsgl4/deferred/gl4lightrenderer.hh>
#include <graphicsgl4/deferred/gl4defaultcollector.hh>
#include <graphicsgl4/deferred/gl4gbuffer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/irendertarget.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csgeometrydata.hh>
#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/graphics/cslight.hh>
#include <cobalt/math/csclipper.hh>


cs::LightRendererGL4::LightRendererGL4(cs::iGraphics *renderer)
  : m_renderer(renderer)
{
  cs::iSampler *depthSampler = m_renderer->CreateSampler();
  depthSampler->SetFilter(cs::eFM_MinMagLinear);
  depthSampler->SetAddressU(cs::eTAM_ClampBorder);
  depthSampler->SetAddressV(cs::eTAM_ClampBorder);
  depthSampler->SetAddressW(cs::eTAM_ClampBorder);
  depthSampler->SetTextureCompareMode(cs::eTCM_CompareToR);
  depthSampler->SetTextureCompareFunc(cs::eTCF_LessOrEqual);
  m_depthSampler = new cs::SamplerWrapper(depthSampler);
}


cs::LightRendererGL4::~LightRendererGL4()
{
  CS_RELEASE(m_depthSampler);
  CS_RELEASE(m_shadowBuffer);
}


void cs::LightRendererGL4::InitializeLightProgram(LightProgram *lightProgram, const cs::ResourceLocator &locator)
{
  lightProgram->program = cs::ResourceManager::Get()->GetOrLoad<cs::ShaderWrapper>(locator);
  if (lightProgram->program && lightProgram->program->IsValid())
  {
    cs::iShader *program = lightProgram->program->Get();
    lightProgram->gbuffer.attrDiffuseRoughness = program->GetAttribute(cs::ShaderAttributeID("DiffuseRoughness"));
    lightProgram->gbuffer.attrNormalLightMode = program->GetAttribute(cs::ShaderAttributeID("NormalLightMode"));
    lightProgram->gbuffer.attrEmissiveMetallic = program->GetAttribute(cs::ShaderAttributeID("EmissiveMetallic"));
    lightProgram->gbuffer.attrSSSSpecular = program->GetAttribute(cs::ShaderAttributeID("SSSSpecular"));
    lightProgram->gbuffer.attrDepth = program->GetAttribute(cs::ShaderAttributeID("Depth"));

    lightProgram->attrColor = program->GetAttribute(cs::ShaderAttributeID("LightColor"));
    lightProgram->attrEnergy = program->GetAttribute(cs::ShaderAttributeID("LightEnergy"));
  }
}


void cs::LightRendererGL4::BindGBuffer(GBufferAttribs &attribs, cs::GBufferGL4 *gbuffer)
{
  if (attribs.attrDiffuseRoughness)
  {
    cs::eTextureUnit unit = m_renderer->BindTexture(gbuffer->GetDiffuseRoughness()->Get());
    attribs.attrDiffuseRoughness->Set(unit);
  }
  if (attribs.attrNormalLightMode)
  {
    cs::eTextureUnit unit = m_renderer->BindTexture(gbuffer->GetNormalLightMode()->Get());
    attribs.attrNormalLightMode->Set(unit);
  }
  if (attribs.attrEmissiveMetallic)
  {
    cs::eTextureUnit unit = m_renderer->BindTexture(gbuffer->GetEmissiveMetallic()->Get());
    attribs.attrEmissiveMetallic->Set(unit);
  }
  if (attribs.attrSSSSpecular)
  {
    cs::eTextureUnit unit = m_renderer->BindTexture(gbuffer->GetSSSSpec()->Get());
    attribs.attrSSSSpecular->Set(unit);
  }
  if (attribs.attrDepth)
  {
    cs::eTextureUnit unit = m_renderer->BindTexture(gbuffer->GetDepth()->Get());
    attribs.attrDepth->Set(unit);
  }
}

void cs::LightRendererGL4::BindLight(LightProgram &lightProgram, cs::Light *light)
{
  if (lightProgram.attrColor)
  {
    lightProgram.attrColor->Set(light->GetColor());
    CS_CHECK_GRAPHICS_ERROR(m_renderer);
  }
  if (lightProgram.attrEnergy)
  {
    lightProgram.attrEnergy->Set(light->GetEnergy());
    CS_CHECK_GRAPHICS_ERROR(m_renderer);
  }
}



void cs::LightRendererGL4::CalcShadowIntensity(const cs::Light *light)
{
  float shadowIntensity = light->GetShadowIntensity();
  m_shadowIntensity.Set(1.0f - shadowIntensity, shadowIntensity);
}





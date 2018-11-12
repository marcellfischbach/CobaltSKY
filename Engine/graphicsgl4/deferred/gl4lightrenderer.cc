
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


csLightRendererGL4::csLightRendererGL4(iGraphics *renderer)
  : m_renderer(renderer)
{
  iSampler *depthSampler = m_renderer->CreateSampler();
  depthSampler->SetFilter(eFM_MinMagLinear);
  depthSampler->SetAddressU(eTAM_ClampBorder);
  depthSampler->SetAddressV(eTAM_ClampBorder);
  depthSampler->SetAddressW(eTAM_ClampBorder);
  depthSampler->SetTextureCompareMode(eTCM_CompareToR);
  depthSampler->SetTextureCompareFunc(eTCF_LessOrEqual);
  m_depthSampler = new csSamplerWrapper(depthSampler);
}


csLightRendererGL4::~csLightRendererGL4()
{
  CS_RELEASE(m_depthSampler);
  CS_RELEASE(m_shadowBuffer);
}


void csLightRendererGL4::InitializeLightProgram(LightProgram *lightProgram, const csResourceLocator &locator)
{
  lightProgram->program = csResourceManager::Get()->GetOrLoad<csShaderWrapper>(locator);
  if (lightProgram->program && lightProgram->program->IsValid())
  {
    iShader *program = lightProgram->program->Get();
    lightProgram->gbuffer.attrDiffuseRoughness = program->GetAttribute(csShaderAttributeID("DiffuseRoughness"));
    lightProgram->gbuffer.attrNormalLightMode = program->GetAttribute(csShaderAttributeID("NormalLightMode"));
    lightProgram->gbuffer.attrEmissiveMetallic = program->GetAttribute(csShaderAttributeID("EmissiveMetallic"));
    lightProgram->gbuffer.attrSSSSpecular = program->GetAttribute(csShaderAttributeID("SSSSpecular"));
    lightProgram->gbuffer.attrDepth = program->GetAttribute(csShaderAttributeID("Depth"));

    lightProgram->attrColor = program->GetAttribute(csShaderAttributeID("LightColor"));
    lightProgram->attrEnergy = program->GetAttribute(csShaderAttributeID("LightEnergy"));
  }
}


void csLightRendererGL4::BindGBuffer(GBufferAttribs &attribs, csGBufferGL4 *gbuffer)
{
  if (attribs.attrDiffuseRoughness)
  {
    csTextureUnit unit = m_renderer->BindTexture(gbuffer->GetDiffuseRoughness()->Get());
    attribs.attrDiffuseRoughness->Set(unit);
  }
  if (attribs.attrNormalLightMode)
  {
    csTextureUnit unit = m_renderer->BindTexture(gbuffer->GetNormalLightMode()->Get());
    attribs.attrNormalLightMode->Set(unit);
  }
  if (attribs.attrEmissiveMetallic)
  {
    csTextureUnit unit = m_renderer->BindTexture(gbuffer->GetEmissiveMetallic()->Get());
    attribs.attrEmissiveMetallic->Set(unit);
  }
  if (attribs.attrSSSSpecular)
  {
    csTextureUnit unit = m_renderer->BindTexture(gbuffer->GetSSSSpec()->Get());
    attribs.attrSSSSpecular->Set(unit);
  }
  if (attribs.attrDepth)
  {
    csTextureUnit unit = m_renderer->BindTexture(gbuffer->GetDepth()->Get());
    attribs.attrDepth->Set(unit);
  }
}

void csLightRendererGL4::BindLight(LightProgram &lightProgram, csLight *light)
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



void csLightRendererGL4::CalcShadowIntensity(const csLight *light)
{
  float shadowIntensity = light->GetShadowIntensity();
  m_shadowIntensity.Set(1.0f - shadowIntensity, shadowIntensity);
}







#include <RenderGL4/Deferred/LightRenderer.hh>
#include <RenderGL4/Deferred/GBuffer.hh>
#include <RenderGL4/DefinesGL4.hh>
#include <RenderGL4/RendererGL4.hh>
#include <RenderGL4/Shader.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Graphics/Light.hh>


vkLightRendererGL4::vkLightRendererGL4()
{
}


vkLightRendererGL4::~vkLightRendererGL4()
{
}


void vkLightRendererGL4::InitializeLightProgram(LightProgram *lightProgram, const vkResourceLocator &locator)
{
  lightProgram->program = vkResourceManager::Get()->GetOrLoad<vkProgramGL4>(locator);
  if (lightProgram->program)
  {
    lightProgram->gbuffer.attrDiffuseRoughness = lightProgram->program->GetAttribute(vkShaderAttributeID ("DiffuseRoughness"));
    lightProgram->gbuffer.attrNormalLightMode = lightProgram->program->GetAttribute(vkShaderAttributeID("NormalLightMode"));
    lightProgram->gbuffer.attrEmissiveMetallic = lightProgram->program->GetAttribute(vkShaderAttributeID("EmissiveMetallic"));
    lightProgram->gbuffer.attrSSSSpecular = lightProgram->program->GetAttribute(vkShaderAttributeID("SSSSpecular"));
    lightProgram->gbuffer.attrDepth = lightProgram->program->GetAttribute(vkShaderAttributeID("Depth"));

    lightProgram->attrColor = lightProgram->program->GetAttribute(vkShaderAttributeID("LightColor"));
    lightProgram->attrEnergy = lightProgram->program->GetAttribute(vkShaderAttributeID("LightEnergy"));
  }
}


void vkLightRendererGL4::BindGBuffer(RendererGL4 *renderer, GBufferAttribs &attribs, vkGBuffer *gbuffer)
{
  if (attribs.attrDiffuseRoughness)
  {
    vkTextureUnit unit = renderer->BindTexture(gbuffer->GetDiffuseRoughness());
    attribs.attrDiffuseRoughness->Set(unit);
  }
  if (attribs.attrNormalLightMode)
  {
    vkTextureUnit unit = renderer->BindTexture(gbuffer->GetNormalLightMode());
    attribs.attrNormalLightMode->Set(unit);
  }
  if (attribs.attrEmissiveMetallic)
  {
    vkTextureUnit unit = renderer->BindTexture(gbuffer->GetEmissiveMetallic());
    attribs.attrEmissiveMetallic->Set(unit);
  }
  if (attribs.attrSSSSpecular)
  {
    vkTextureUnit unit = renderer->BindTexture(gbuffer->GetSSSSpec());
    attribs.attrSSSSpecular->Set(unit);
  }
  if (attribs.attrDepth)
  {
    vkTextureUnit unit = renderer->BindTexture(gbuffer->GetDepth());
    attribs.attrDepth->Set(unit);
  }
}

void vkLightRendererGL4::BindLight(RendererGL4 *renderer, LightProgram &lightProgram, vkLight *light)
{
  if (lightProgram.attrColor)
  {
    lightProgram.attrColor->Set(light->GetColor());
    VK_CHECK_GL_ERROR;
  }
  if (lightProgram.attrEnergy)
  {
    lightProgram.attrEnergy->Set(light->GetEnergy());
    VK_CHECK_GL_ERROR;
  }
}



vkDirectionalLightRendererGL4::vkDirectionalLightRendererGL4()
  : vkLightRendererGL4()
{
  InitializeLightProgram(&m_programWithoutShadow, vkResourceLocator("${shaders}/deferred/deferred.xml", "DirectionalLight"));

  m_attrLightDirection = m_programWithoutShadow.program->GetAttribute(vkShaderAttributeID("LightDirection"));
}

vkDirectionalLightRendererGL4::~vkDirectionalLightRendererGL4()
{

}




void vkDirectionalLightRendererGL4::Render(RendererGL4 *renderer, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target)
{
  vkDirectionalLight *directionalLight = static_cast<vkDirectionalLight*>(light);
  // no the final image will be assembled
  renderer->SetRenderTarget(target);

  // from now on we will only render to the single color buffer
  GLenum buffers[] = {
    GL_COLOR_ATTACHMENT0,
  };
  glDrawBuffers(1, buffers);

  LightProgram &prog = m_programWithoutShadow;
  renderer->SetShader(prog.program);
  // bind the gbuffer this is used by the light program
  BindGBuffer(renderer, prog.gbuffer, gbuffer);
  BindLight(renderer, prog, light);

  BindDirectionalLight(renderer, directionalLight);

  renderer->RenderFullScreenFrame();
}


void vkDirectionalLightRendererGL4::BindDirectionalLight(RendererGL4 *renderer, vkDirectionalLight *directionalLight)
{
  if (m_attrLightDirection)
  {
    m_attrLightDirection->Set(directionalLight->GetDirection());
  }

}






vkPointLightRendererGL4::vkPointLightRendererGL4()
  : vkLightRendererGL4()
{
  InitializeLightProgram(&m_programWithoutShadow, vkResourceLocator("${shaders}/deferred/deferred.xml", "PointLight"));

  m_attrLightPosition = m_programWithoutShadow.program->GetAttribute(vkShaderAttributeID("LightPosition"));
  m_attrLightRange = m_programWithoutShadow.program->GetAttribute(vkShaderAttributeID("LightRadius"));
}

vkPointLightRendererGL4::~vkPointLightRendererGL4()
{

}




void vkPointLightRendererGL4::Render(RendererGL4 *renderer, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target)
{
  vkPointLight *pointLight = static_cast<vkPointLight*>(light);
  // no the final image will be assembled
  renderer->SetRenderTarget(target);

  // from now on we will only render to the single color buffer
  GLenum buffers[] = {
    GL_COLOR_ATTACHMENT0,
  };
  glDrawBuffers(1, buffers);

  LightProgram &prog = m_programWithoutShadow;
  renderer->SetShader(prog.program);
  // bind the gbuffer this is used by the light program
  BindGBuffer(renderer, prog.gbuffer, gbuffer);
  BindLight(renderer, prog, light);

  BindPointLight(renderer, pointLight);

  renderer->RenderFullScreenFrame();
}


void vkPointLightRendererGL4::BindPointLight(RendererGL4 *renderer, vkPointLight *pointLight)
{
  if (m_attrLightPosition)
  {
    m_attrLightPosition->Set(pointLight->GetPosition());
  }
  if (m_attrLightRange)
  {
    m_attrLightRange->Set(pointLight->GetRadius());
  }
}


#include <RenderGL4/Deferred/LightRenderer.hh>
#include <RenderGL4/Deferred/DefaultCollector.hh>
#include <RenderGL4/Deferred/GBuffer.hh>
#include <RenderGL4/DefinesGL4.hh>
#include <RenderGL4/RendererGL4.hh>
#include <RenderGL4/RenderTargetGL4.hh>
#include <RenderGL4/Shader.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Graphics/Camera.hh>
#include <Valkyrie/Graphics/Light.hh>
#include <Valkyrie/Graphics/Scene/GeometryNode.hh>
#include <Valkyrie/Graphics/Scene/Node.hh>


vkLightRendererGL4::vkLightRendererGL4(RendererGL4 *renderer)
  : m_renderer(renderer)
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


void vkLightRendererGL4::BindGBuffer(GBufferAttribs &attribs, vkGBuffer *gbuffer)
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

void vkLightRendererGL4::BindLight(LightProgram &lightProgram, vkLight *light)
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



vkDirectionalLightRendererGL4::vkDirectionalLightRendererGL4(RendererGL4 *renderer)
  : vkLightRendererGL4(renderer)
{
  InitializeLightProgram(&m_programWithoutShadow, vkResourceLocator("${shaders}/deferred/deferred.xml", "DirectionalLight"));

  m_attrLightDirection = m_programWithoutShadow.program->GetAttribute(vkShaderAttributeID("LightDirection"));

  vkUInt16 bufferSize = 512;
  ITexture2DArray *colorBuffer = renderer->CreateTexture2DArray(ePF_RGBA, bufferSize, bufferSize, 3);
  ITexture2DArray *depthBuffer = renderer->CreateTexture2DArray(ePF_D24S8, bufferSize, bufferSize, 3);

  m_shadowBuffer = static_cast<vkRenderTargetGL4*>(renderer->CreateRenderTarget());
  m_shadowBuffer->Initialize();
  m_shadowBuffer->AddColorTexture(colorBuffer);
  m_shadowBuffer->SetDepthTexture(depthBuffer);
  m_shadowBuffer->Finilize();

}

vkDirectionalLightRendererGL4::~vkDirectionalLightRendererGL4()
{

}




void vkDirectionalLightRendererGL4::Render(vkNode *node, const vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target)
{
  vkDirectionalLight *directionalLight = static_cast<vkDirectionalLight*>(light);

  if (directionalLight->IsCastingShadow())
  {
    //RenderShadow(node, camera, directionalLight);
  }



  // now the final image will be assembled
  m_renderer->SetRenderTarget(target);

  // from now on we will only render to the single color buffer
  GLenum buffers[] = {
    GL_COLOR_ATTACHMENT0,
  };
  glDrawBuffers(1, buffers);

  LightProgram &prog = m_programWithoutShadow;
  m_renderer->SetShader(prog.program);
  // bind the gbuffer this is used by the light program
  BindGBuffer(prog.gbuffer, gbuffer);
  BindLight(prog, light);

  BindDirectionalLight(directionalLight);

  m_renderer->RenderFullScreenFrame();
}


void vkDirectionalLightRendererGL4::BindDirectionalLight(vkDirectionalLight *directionalLight)
{
  if (m_attrLightDirection)
  {
    m_attrLightDirection->Set(directionalLight->GetDirection());
  }

}


void vkDirectionalLightRendererGL4::RenderShadow(vkNode *node, const vkCamera *camera, const vkDirectionalLight *light)
{
  CalcPSSMMatrices(light, camera);

  // collect the shadow casting objects
  vkDefaultCollector collector(&m_geometries, 0);
  m_geometries.Clear();
  node->Scan(0, m_renderer, &collector);

  // setup the rendering 
  m_renderer->SetRenderTarget(m_shadowBuffer);
  m_renderer->Clear();
  m_renderer->SetShadowMatrices(m_shadowProjView, 3);
  m_renderer->SetBlendEnabled(false);

  GLenum buffers[] = {
    GL_COLOR_ATTACHMENT0, // Color
  };
  glDrawBuffers(1, buffers);

  // render all geometries
  for (vkSize i = 0; i < m_geometries.length; ++i)
  {
    vkGeometryNode *geometryNode = m_geometries[i];
    if (geometryNode)
    {
      geometryNode->Render(m_renderer, eRP_GBuffer);
    }
  }

}

void vkDirectionalLightRendererGL4::CalcPSSMMatrices(const vkDirectionalLight *light, const vkCamera *camera)
{
  float dists[] = { 1.0f, 15.0f, 30.0f, 100.0f };
  vkVector3f points[8];

  for (vkSize i = 0; i < 3; ++i)
  {
    camera->GetPlanePoints(dists[i], &points[0]);
    camera->GetPlanePoints(dists[i+1], &points[4]);
    CalcMatrix(light->GetDirection(), 8, points, m_shadowCam[i], m_shadowCam[i]);
    vkMatrix4f::Mult(m_shadowProj[i], m_shadowCam[i], m_shadowProjView[i]);
  }
}

void vkDirectionalLightRendererGL4::CalcMatrix(const vkVector3f &dir, vkSize numPoints, vkVector3f *points, vkMatrix4f &cam, vkMatrix4f &proj) const
{
  vkVector3f spot;
  for (vkSize i = 0; i < numPoints; i++)
  {
    vkVector3f::Add(spot, points[i], spot);
  }

  vkVector3f eye;
  vkVector3f::Mul(dir, -100.0f, eye);
  vkVector3f::Add(eye, spot, eye);

  vkVector3f up(0.0f, 0.0f, 1.0f);
  if (dir.z >= 0.9999999)
  {
    up.Set(1.0f, 0.0f, 0.0f);
  }
  else if (dir.z <= -0.9999999)
  {
    up.Set(-1.0f, 0.0f, 0.0f);
  }

  cam.SetLookAt(eye, spot, up);

  vkVector2f min(FLT_MAX, FLT_MAX);
  vkVector2f max(-FLT_MAX, -FLT_MAX);
  vkVector3f t;
  for (vkSize i = 0; i < numPoints; i++)
  {
    vkVector3f &p = points[i];
    vkMatrix4f::Transform(cam, p, t);
    if (min.x > p.x)
    {
      min.x = p.x;
    }
    if (min.y > p.y)
    {
      min.y = p.y;
    }
    if (max.x < p.x)
    {
      max.x = p.x;
    }
    if (max.y < p.y)
    {
      max.y = p.y;
    }
  }
  proj.SetOrthographic(min.x, max.x, min.y, max.y, 1.0f, 1024.0f);

}





vkPointLightRendererGL4::vkPointLightRendererGL4(RendererGL4 *renderer)
  : vkLightRendererGL4(renderer)
{
  InitializeLightProgram(&m_programWithoutShadow, vkResourceLocator("${shaders}/deferred/deferred.xml", "PointLight"));

  m_attrLightPosition = m_programWithoutShadow.program->GetAttribute(vkShaderAttributeID("LightPosition"));
  m_attrLightRange = m_programWithoutShadow.program->GetAttribute(vkShaderAttributeID("LightRadius"));
}

vkPointLightRendererGL4::~vkPointLightRendererGL4()
{

}




void vkPointLightRendererGL4::Render(vkNode *node, const vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target)
{
  vkPointLight *pointLight = static_cast<vkPointLight*>(light);
  // no the final image will be assembled
  m_renderer->SetRenderTarget(target);

  // from now on we will only render to the single color buffer
  GLenum buffers[] = {
    GL_COLOR_ATTACHMENT0,
  };
  glDrawBuffers(1, buffers);

  LightProgram &prog = m_programWithoutShadow;
  m_renderer->SetShader(prog.program);
  // bind the gbuffer this is used by the light program
  BindGBuffer(prog.gbuffer, gbuffer);
  BindLight(prog, light);

  BindPointLight(pointLight);

  m_renderer->RenderFullScreenFrame();
}





void vkPointLightRendererGL4::BindPointLight(vkPointLight *pointLight)
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


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
  m_depthSampler = m_renderer->CreateSampler();
  m_depthSampler->SetFilter(eFM_MinMagLinear);
  m_depthSampler->SetAddressU(eTAM_ClampBorder);
  m_depthSampler->SetAddressV(eTAM_ClampBorder);
  m_depthSampler->SetAddressW(eTAM_ClampBorder);
  m_depthSampler->SetTextureCompareMode(eTCM_CompareToR);
  m_depthSampler->SetTextureCompareFunc(eTCF_Less);
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
  InitializeLightProgram(&m_programNoShadow, vkResourceLocator("${shaders}/deferred/deferred.xml", "DirectionalLight"));
  m_attrLightDirectionNoShadow = m_programNoShadow.program->GetAttribute(vkShaderAttributeID("LightDirection"));

  InitializeLightProgram(&m_programPSSM, vkResourceLocator("${shaders}/deferred/deferred.xml", "DirectionalLightPSSM"));
  m_attrLightDirectionPSSM = m_programPSSM.program->GetAttribute(vkShaderAttributeID("LightDirection"));
  m_attrDisancesPSSM= m_programPSSM.program->GetAttribute(vkShaderAttributeID("Distances"));
  m_attrShadowMats = m_programPSSM.program->GetAttribute(vkShaderAttributeID("ShadowMats"));
  m_attrShadowMap = m_programPSSM.program->GetAttribute(vkShaderAttributeID("ShadowMap"));
  m_attrMapBias = m_programPSSM.program->GetAttribute(vkShaderAttributeID("MapBias"));
  m_attrShadowIntensity = m_programPSSM.program->GetAttribute(vkShaderAttributeID("ShadowIntensity"));

  m_distances = vkVector3f(15.0f, 45.0f, 135.0f);
  m_mapBias = 0.99f;

  vkUInt16 bufferSize = 1024;
  ITexture2DArray *colorBuffer = renderer->CreateTexture2DArray(ePF_RGBA, bufferSize, bufferSize, 3);
  m_depthBuffer = renderer->CreateTexture2DArray(ePF_D24S8, bufferSize, bufferSize, 3);
  
  colorBuffer->SetSampler(vkGBuffer::GetColorSampler(renderer));
  m_depthBuffer->SetSampler(m_depthSampler);

  m_shadowBuffer = static_cast<vkRenderTargetGL4*>(renderer->CreateRenderTarget());
  m_shadowBuffer->Initialize(bufferSize, bufferSize);
  m_shadowBuffer->AddColorTexture(colorBuffer);
  m_shadowBuffer->SetDepthTexture(m_depthBuffer);
  m_shadowBuffer->Finilize();

}

vkDirectionalLightRendererGL4::~vkDirectionalLightRendererGL4()
{

}




void vkDirectionalLightRendererGL4::Render(vkNode *node, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target)
{
  vkBlendMode blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha;
  m_renderer->GetBlendMode(blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha);
  bool blendEnabled = m_renderer->IsBlendEnabled();

  vkDirectionalLight *directionalLight = static_cast<vkDirectionalLight*>(light);

  bool shadow = directionalLight->IsCastingShadow();
  if (shadow)
  {
    RenderShadow(node, camera, directionalLight);
  }



  // now the final image will be assembled
  m_renderer->SetRenderTarget(target);
  m_renderer->SetViewport(target);
  m_renderer->SetBlendEnabled(blendEnabled);
  m_renderer->SetBlendMode(blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha);

  // from now on we will only render to the single color buffer
  GLenum buffers[] = {
    GL_COLOR_ATTACHMENT0,
  };
  glDrawBuffers(1, buffers);

  LightProgram &prog = shadow ? m_programPSSM : m_programNoShadow;
  m_renderer->SetShader(prog.program);
  m_renderer->InvalidateTextures();
  // bind the gbuffer this is used by the light program
  BindGBuffer(prog.gbuffer, gbuffer);
  BindLight(prog, light);

  if (shadow)
  {
    BindDirectionalLightPSSM(directionalLight);
  }
  else
  {
    BindDirectionalLightNoShadow(directionalLight);
  }

  m_renderer->RenderFullScreenFrame();
}


void vkDirectionalLightRendererGL4::BindDirectionalLightNoShadow(vkDirectionalLight *directionalLight)
{
  if (m_attrLightDirectionNoShadow)
  {
    m_attrLightDirectionNoShadow->Set(directionalLight->GetDirection());
  }

}



void vkDirectionalLightRendererGL4::BindDirectionalLightPSSM(vkDirectionalLight *directionalLight)
{
  if (m_attrLightDirectionPSSM)
  {
    m_attrLightDirectionPSSM->Set(directionalLight->GetDirection());
  }
  if (m_attrDisancesPSSM)
  {
    m_attrDisancesPSSM->Set(m_distances);
  }
  if (m_attrShadowMats)
  {
    m_attrShadowMats->Set(m_shadowProjView, 3);
  }
  if (m_attrShadowMap)
  {
    vkTextureUnit tu = m_renderer->BindTexture(m_depthBuffer);
    m_attrShadowMap->Set(tu);
  }
  if (m_attrMapBias)
  {
    m_attrMapBias->Set(m_mapBias);
  }
  if (m_attrShadowIntensity)
  {
    m_attrShadowIntensity->Set(m_shadowIntensity);
  }
}


void vkDirectionalLightRendererGL4::RenderShadow(vkNode *node, const vkCamera *camera, const vkDirectionalLight *light)
{
  CalcPSSMMatrices(light, camera);

  float shadowIntensity = light->GetShadowIntensity();
  m_shadowIntensity.Set(1.0 - shadowIntensity, shadowIntensity);

  // collect the shadow casting objects
  vkDefaultCollector collector(&m_geometries, 0);
  m_geometries.Clear();
  node->Scan(0, m_renderer, &collector);

  // setup the rendering 
  m_renderer->SetRenderTarget(m_shadowBuffer);
  m_renderer->SetViewport(m_shadowBuffer);
  GLenum buffers[] = {
    GL_COLOR_ATTACHMENT0, // Color
  };
  glDrawBuffers(1, buffers);
  glDepthMask(true);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0);
  glColorMask(true, true, true, true);
  //glDisable(GL_CULL_FACE);

  m_renderer->Clear();
  m_renderer->SetShadowMatrices(m_shadowProjView, 3);
  m_renderer->SetBlendEnabled(false);

  // render all geometries
  for (vkSize i = 0; i < m_geometries.length; ++i)
  {
    vkGeometryNode *geometryNode = m_geometries[i];
    if (geometryNode)
    {
      geometryNode->Render(m_renderer, eRP_ShadowPSSM);
    }
  }
  glColorMask(true, true, true, true);

}

void vkDirectionalLightRendererGL4::CalcPSSMMatrices(const vkDirectionalLight *light, const vkCamera *camera)
{
  float dists[] = { 0.0f, m_distances.x, m_distances.y, m_distances.z };
  vkVector3f points[8];

  for (vkSize i = 0; i < 3; ++i)
  {
    camera->GetPlanePoints(0, &points[i]);
    camera->GetPlanePoints(dists[i+1], &points[4]);
    CalcMatrix(light->GetDirection(), 8, points, m_shadowCam[i], m_shadowProj[i]);
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
  vkVector3f::Div(spot, (float)numPoints, spot);

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

  cam.SetLookAt(vkVector3f(0, 0, 0), dir, up);

  vkVector3f min(FLT_MAX, FLT_MAX, FLT_MAX);
  vkVector3f max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
  vkVector3f t;
  for (vkSize i = 0; i < numPoints; i++)
  {
    vkVector3f &p = points[i];
    vkMatrix4f::Transform(cam, p, t);
    if (min.x > t.x)
    {
      min.x = t.x;
    }
    if (min.y > t.y)
    {
      min.y = t.y;
    }
    if (min.z > t.z)
    {
      min.z = t.z;
    }
    if (max.x < t.x)
    {
      max.x = t.x;
    }
    if (max.y < t.y)
    {
      max.y = t.y;
    }
    if (max.z < t.z)
    {
      max.z = t.z;
    }
  }
  m_renderer->GetOrthographicProjection(min.x, max.x, min.z, max.z, min.y, max.y, proj);
  // proj.SetOrthographic(min.x, max.x, min.y, max.y, max.z, min.z);
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




void vkPointLightRendererGL4::Render(vkNode *node, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target)
{
  vkPointLight *pointLight = static_cast<vkPointLight*>(light);
  // no the final image will be assembled
  m_renderer->SetRenderTarget(target);
  m_renderer->SetViewport(target);

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
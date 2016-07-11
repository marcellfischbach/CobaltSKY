

#include <GraphicsGL4/Deferred/LightRenderer.hh>
#include <GraphicsGL4/Deferred/DefaultCollector.hh>
#include <GraphicsGL4/Deferred/GBuffer.hh>
#include <GraphicsGL4/DefinesGL4.hh>
#include <GraphicsGL4/GraphicsGL4.hh>
#include <GraphicsGL4/RenderTargetGL4.hh>
#include <GraphicsGL4/Shader.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/Geometry.hh>
#include <Valkyrie/Entity/RenderState.hh>
#include <Valkyrie/Graphics/Camera.hh>
#include <Valkyrie/Graphics/Light.hh>
#include <Valkyrie/Math/Clipper.hh>


vkLightvkGraphicsGL4::vkLightvkGraphicsGL4(vkGraphicsGL4 *renderer)
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


vkLightvkGraphicsGL4::~vkLightvkGraphicsGL4()
{
  VK_RELEASE(m_depthSampler);
  VK_RELEASE(m_shadowBuffer);
}


void vkLightvkGraphicsGL4::InitializeLightProgram(LightProgram *lightProgram, const vkResourceLocator &locator)
{
  lightProgram->program = vkResourceManager::Get()->GetOrLoad<vkProgramGL4>(locator);
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


void vkLightvkGraphicsGL4::BindGBuffer(GBufferAttribs &attribs, vkGBuffer *gbuffer)
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

void vkLightvkGraphicsGL4::BindLight(LightProgram &lightProgram, vkLight *light)
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



void vkLightvkGraphicsGL4::CalcShadowIntensity(const vkLight *light)
{
  float shadowIntensity = light->GetShadowIntensity();
  m_shadowIntensity.Set(1.0f - shadowIntensity, shadowIntensity);
}







vkDirectionalLightvkGraphicsGL4::vkDirectionalLightvkGraphicsGL4(vkGraphicsGL4 *renderer)
  : vkLightvkGraphicsGL4(renderer)
  , m_colorBuffer(0)
  , m_colorBufferBlur(0)
  , m_depthBuffer(0)
{
  InitializeLightProgram(&m_programNoShadow, vkResourceLocator("${shaders}/deferred/deferred.xml", "DirectionalLight"));
  m_attrLightDirectionNoShadow = m_programNoShadow.program->GetAttribute(vkShaderAttributeID("LightDirection"));

  InitializeLightProgram(&m_programPSSM, vkResourceLocator("${shaders}/deferred/deferred.xml", "DirectionalLightPSSM"));
  m_attrLightDirectionPSSM = m_programPSSM.program->GetAttribute(vkShaderAttributeID("LightDirection"));
  m_attrDisancesPSSM = m_programPSSM.program->GetAttribute(vkShaderAttributeID("Distances"));
  m_attrShadowMatsProjView = m_programPSSM.program->GetAttribute(vkShaderAttributeID("ShadowMatsProjView"));
  m_attrShadowMatsProj = m_programPSSM.program->GetAttribute(vkShaderAttributeID("ShadowMatsProj"));
  m_attrShadowMatsView = m_programPSSM.program->GetAttribute(vkShaderAttributeID("ShadowMatsView"));
  m_attrShadowProjNearFar = m_programPSSM.program->GetAttribute(vkShaderAttributeID("ShadowProjNearFar"));
  m_attrShadowMap = m_programPSSM.program->GetAttribute(vkShaderAttributeID("ShadowMap"));
  m_attrShadowColorMap = m_programPSSM.program->GetAttribute(vkShaderAttributeID("ShadowColorMap"));
  m_attrShadowMapSizeInv = m_programPSSM.program->GetAttribute(vkShaderAttributeID("ShadowMapSizeInv"));
  m_attrMapBias = m_programPSSM.program->GetAttribute(vkShaderAttributeID("MapBias"));
  m_attrShadowIntensity = m_programPSSM.program->GetAttribute(vkShaderAttributeID("ShadowIntensity"));



  m_distances = vkVector3f(15.0f, 45.0f, 135.0f);

  vkPixelFormat shadowBufferFormat = ePF_R16G16F;
  m_shadowBufferSize = 1024;
  m_colorBuffer = renderer->CreateTexture2DArray(shadowBufferFormat, m_shadowBufferSize, m_shadowBufferSize, 3);
  m_depthBuffer = renderer->CreateTexture2DArray(ePF_D24S8, m_shadowBufferSize, m_shadowBufferSize, 3);

  ISampler *colorSampler = renderer->CreateSampler();
  colorSampler->SetFilter(eFM_MinMagLinear);
  colorSampler->SetAddressU(eTAM_ClampBorder);
  colorSampler->SetAddressV(eTAM_ClampBorder);
  colorSampler->SetAddressW(eTAM_ClampBorder);

  m_colorBuffer->SetSampler(colorSampler);
  m_depthBuffer->SetSampler(m_depthSampler);
  
  colorSampler->Release();

  m_shadowBuffer = static_cast<vkRenderTargetGL4*>(renderer->CreateRenderTarget());
  m_shadowBuffer->Initialize(m_shadowBufferSize, m_shadowBufferSize);
  m_shadowBuffer->AddColorTexture(m_colorBuffer);
  m_shadowBuffer->SetDepthTexture(m_depthBuffer);
  m_shadowBuffer->Finilize();

}

vkDirectionalLightvkGraphicsGL4::~vkDirectionalLightvkGraphicsGL4()
{
  VK_RELEASE(m_colorBuffer);
  VK_RELEASE(m_depthBuffer);
  VK_RELEASE(m_colorBufferBlur);
}




void vkDirectionalLightvkGraphicsGL4::Render(vkEntity *root, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target)
{
  vkBlendMode blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha;
  m_renderer->GetBlendMode(blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha);
  bool blendEnabled = m_renderer->IsBlendEnabled();

  vkDirectionalLight *directionalLight = static_cast<vkDirectionalLight*>(light);

  bool shadow = directionalLight->IsCastingShadow();
  if (shadow)
  {
    RenderShadow(root, camera, directionalLight);
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


void vkDirectionalLightvkGraphicsGL4::BindDirectionalLightNoShadow(vkDirectionalLight *directionalLight)
{
  if (m_attrLightDirectionNoShadow)
  {
    m_attrLightDirectionNoShadow->Set(directionalLight->GetDirection());
  }

}



void vkDirectionalLightvkGraphicsGL4::BindDirectionalLightPSSM(vkDirectionalLight *directionalLight)
{
  if (m_attrLightDirectionPSSM)
  {
    m_attrLightDirectionPSSM->Set(directionalLight->GetDirection());
  }
  if (m_attrDisancesPSSM)
  {
    m_attrDisancesPSSM->Set(m_distances);
  }
  if (m_attrShadowMatsProjView)
  {
    m_attrShadowMatsProjView->Set(m_shadowProjView, 3);
  }
  if (m_attrShadowMatsProj)
  {
    m_attrShadowMatsProj->Set(m_shadowProj, 3);
  }
  if (m_attrShadowMatsView)
  {
    m_attrShadowMatsView->Set(m_shadowCam, 3);
  }
  if (m_attrShadowMap)
  {
    vkTextureUnit tu = m_renderer->BindTexture(m_depthBuffer);
    m_attrShadowMap->Set(tu);
  }
  if (m_attrShadowColorMap)
  {
    vkTextureUnit tu = m_renderer->BindTexture(m_colorBuffer);
    m_attrShadowColorMap->Set(tu);
  }
  if (m_attrMapBias)
  {
    m_attrMapBias->Set(m_mapBias);
  }
  if (m_attrShadowIntensity)
  {
    m_attrShadowIntensity->Set(m_shadowIntensity);
  }
  if (m_attrShadowProjNearFar)
  {
    vkVector2f nearFar[3];
    nearFar[0] = vkVector2f(m_min[0].y, m_max[0].y);
    nearFar[1] = vkVector2f(m_min[1].y, m_max[1].y);
    nearFar[2] = vkVector2f(m_min[2].y, m_max[2].y);
    m_attrShadowProjNearFar->Set(nearFar, 3);
  }
  if (m_attrShadowMapSizeInv)
  {
    m_attrShadowMapSizeInv->Set(1.0f / m_shadowBufferSize);
  }

}



void vkDirectionalLightvkGraphicsGL4::UpdateProjectionMatrices()
{
  float min = FLT_MAX;
  float max = -FLT_MAX;
  const vkMatrix4f &view = m_shadowCamAll;
  for (vkSize i = 0; i < m_renderStates.length; ++i)
  {
    vkRenderState *renderState = m_renderStates[i];
    if (renderState)
    {
      const vkBoundingBox &bbox = renderState->GetBoundingBox();
      for (unsigned i = 0; i < 8; ++i)
      {
        vkVector3f p;
        vkMatrix4f::Transform(view, bbox.GetPoints()[i], p);
        if (p.y < min)
        {
          min = p.y;
        }
        if (p.y > max)
        {
          max = p.y;
        }
      }
    }
  }
  for (unsigned i = 0; i < 3; ++i)
  {
    m_min[i].y = min;
    m_shadowNearFar[i].Set(m_min[i].y, m_max[i].y);
    m_renderer->GetOrthographicProjection(m_min[i].x, m_max[i].x, m_min[i].z, m_max[i].z, m_min[i].y, m_max[i].y, m_shadowProj[i]);
    vkMatrix4f::Mult(m_shadowProj[i], m_shadowCam[i], m_shadowProjView[i]);

  }
}

vkClipper *vkDirectionalLightvkGraphicsGL4::CreateClipper()
{
  vkVector3f topLeft(m_min[2].x, 0.0, m_max[2].z);
  vkVector3f topRight(m_max[2].x, 0.0, m_max[2].z);
  vkVector3f bottomLeft(m_min[2].x, 0.0, m_min[2].z);
  vkVector3f bottomRight(m_max[2].x, 0.0, m_min[2].z);

  const vkMatrix4f &camInv = m_shadowCamInv[2];
  vkVector3f tl, tr, bl, br;
  vkMatrix4f::Transform(camInv, topLeft, tl);
  vkMatrix4f::Transform(camInv, topRight, tr);
  vkMatrix4f::Transform(camInv, bottomLeft, bl);
  vkMatrix4f::Transform(camInv, bottomRight, br);

  vkVector3f r;
  camInv.GetXAxis(r);
  vkVector3f l(r);
  l *= -1.0f;

  vkVector3f t;
  camInv.GetZAxis(t);
  vkVector3f b(t);
  b *= -1.0f;


  vkPlaneClipper *clipper = new vkPlaneClipper();
  clipper->AddPlane(vkPlane(tl, r));
  clipper->AddPlane(vkPlane(tl, b));
  clipper->AddPlane(vkPlane(br, l));
  clipper->AddPlane(vkPlane(br, t));
  return clipper;
}

void vkDirectionalLightvkGraphicsGL4::RenderShadow(vkEntity *root, vkCamera *camera, const vkDirectionalLight *light)
{
  CalcPSSMMatrices(light, camera);

  CalcShadowIntensity(light);

  vkScanConfig config;
  config.ScanShadowCasters = true;
  config.ScanNonShadowCasters = false;
  config.MainCameraPosition = camera->GetEye();
  // collect the shadow casting objects
  vkDefaultCollector collector(&m_renderStates, &m_renderStates, &m_renderStates, 0, 0);
  m_renderStates.Clear();

  vkClipper *clipper = CreateClipper();
  root->Scan(clipper, m_renderer, &collector, config);
  delete clipper;


  UpdateProjectionMatrices();

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
  glColorMask(true, true, false, false);


  m_renderer->Clear(true, vkVector4f(m_max[2].y, m_max[2].y * m_max[2].y, 1, 1));
  m_renderer->SetShadowMatrices(m_shadowProjView, m_shadowProj, m_shadowCam, m_shadowNearFar, 3);
  m_renderer->SetBlendEnabled(false);

  for (vkSize i = 0; i < m_renderStates.length; ++i)
  {
    vkRenderState *renderState = m_renderStates[i];
    if (renderState)
    {
      renderState->Render(m_renderer, eRP_ShadowPSSM);
    }
  }
  glColorMask(true, true, true, true);

}

void vkDirectionalLightvkGraphicsGL4::CalcPSSMMatrices(const vkDirectionalLight *light, const vkCamera *camera)
{
  float dists[] = { 0.0f, m_distances.x, m_distances.y, m_distances.z };
  vkVector3f points[8];

  for (vkSize i = 0; i < 3; ++i)
  {
    camera->GetPlanePoints(0, &points[0]);
    camera->GetPlanePoints(dists[i + 1], &points[4]);
    CalcMatrix(light->GetDirection(), 8, points, m_shadowCam[i], m_shadowCamInv[i], m_min[i], m_max[i]);
  }

  camera->GetPlanePoints(0, &points[0]);
  camera->GetPlanePoints(m_distances.z, &points[4]);
  CalcMatrix(light->GetDirection(), 8, points, m_shadowCamAll, m_shadowCamInvAll, m_minAll, m_maxAll);
}

void vkDirectionalLightvkGraphicsGL4::CalcMatrix(const vkVector3f &dir, vkSize numPoints, vkVector3f *points, vkMatrix4f &cam, vkMatrix4f &camInv, vkVector3f &min, vkVector3f &max) const
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
  camInv.SetLookAtInv(vkVector3f(0, 0, 0), dir, up);

  min.Set(FLT_MAX, FLT_MAX, FLT_MAX);
  max.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
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
}



vkPointLightvkGraphicsGL4::vkPointLightvkGraphicsGL4(vkGraphicsGL4 *renderer)
  : vkLightvkGraphicsGL4(renderer)
{
  InitializeLightProgram(&m_programNoShadow, vkResourceLocator("${shaders}/deferred/deferred.xml", "PointLight"));
  m_attrLightPositionNoShadow = m_programNoShadow.program->GetAttribute(vkShaderAttributeID("LightPosition"));
  m_attrLightRangeNoShadow = m_programNoShadow.program->GetAttribute(vkShaderAttributeID("LightRadius"));

  InitializeLightProgram(&m_programCubeShadow, vkResourceLocator("${shaders}/deferred/deferred.xml", "PointLightCubeShadow"));
  m_attrLightPositionCubeShadow = m_programCubeShadow.program->GetAttribute(vkShaderAttributeID("LightPosition"));
  m_attrLightRangeCubeShadow = m_programCubeShadow.program->GetAttribute(vkShaderAttributeID("LightRadius"));
  m_attrShadowMats = m_programCubeShadow.program->GetAttribute(vkShaderAttributeID("ShadowMats"));
  m_attrShadowMap = m_programCubeShadow.program->GetAttribute(vkShaderAttributeID("ShadowMap"));
  m_attrMapBias = m_programCubeShadow.program->GetAttribute(vkShaderAttributeID("MapBias"));
  m_attrShadowIntensity = m_programCubeShadow.program->GetAttribute(vkShaderAttributeID("ShadowIntensity"));


  m_mapBias = 0.99f;

  vkUInt16 bufferSize = 1024;
  ITexture2DArray *colorBuffer = renderer->CreateTexture2DArray(ePF_RGBA, bufferSize, bufferSize, 6);
  m_depthBuffer = renderer->CreateTexture2DArray(ePF_D24S8, bufferSize, bufferSize, 6);

  colorBuffer->SetSampler(vkGBuffer::GetColorSampler(renderer));
  m_depthBuffer->SetSampler(m_depthSampler);

  m_shadowBuffer = static_cast<vkRenderTargetGL4*>(renderer->CreateRenderTarget());
  m_shadowBuffer->Initialize(bufferSize, bufferSize);
  m_shadowBuffer->AddColorTexture(colorBuffer);
  m_shadowBuffer->SetDepthTexture(m_depthBuffer);
  m_shadowBuffer->Finilize();

  colorBuffer->Release();


}

vkPointLightvkGraphicsGL4::~vkPointLightvkGraphicsGL4()
{
  VK_RELEASE(m_depthBuffer);
}




void vkPointLightvkGraphicsGL4::Render(vkEntity *root, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target)
{
  vkBlendMode blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha;
  m_renderer->GetBlendMode(blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha);
  bool blendEnabled = m_renderer->IsBlendEnabled();

  vkPointLight *pointLight = static_cast<vkPointLight*>(light);


  bool shadow = pointLight->IsCastingShadow();
  if (shadow)
  {
    RenderShadow(root, pointLight);
  }

  // no the final image will be assembled
  m_renderer->SetRenderTarget(target);
  m_renderer->SetViewport(target);
  m_renderer->SetBlendEnabled(blendEnabled);
  m_renderer->SetBlendMode(blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha);

  // from now on we will only render to the single color buffer
  GLenum buffers[] = {
    GL_COLOR_ATTACHMENT0,
  };
  glDrawBuffers(1, buffers);

  LightProgram &prog = shadow ? m_programCubeShadow : m_programNoShadow;
  m_renderer->SetShader(prog.program);
  // bind the gbuffer this is used by the light program
  BindGBuffer(prog.gbuffer, gbuffer);
  BindLight(prog, light);

  if (shadow)
  {
    BindPointLightCubeShadow(pointLight);
  }
  else
  {
    BindPointLightNo(pointLight);
  }

  m_renderer->RenderFullScreenFrame();
}





void vkPointLightvkGraphicsGL4::BindPointLightNo(vkPointLight *pointLight)
{
  if (m_attrLightPositionNoShadow)
  {
    m_attrLightPositionNoShadow->Set(pointLight->GetPosition());
  }
  if (m_attrLightRangeNoShadow)
  {
    m_attrLightRangeNoShadow->Set(pointLight->GetRadius());
  }
}


void vkPointLightvkGraphicsGL4::BindPointLightCubeShadow(vkPointLight *pointLight)
{
  if (m_attrLightPositionCubeShadow)
  {
    m_attrLightPositionCubeShadow->Set(pointLight->GetPosition());
  }
  if (m_attrLightRangeCubeShadow)
  {
    m_attrLightRangeCubeShadow->Set(pointLight->GetRadius());
  }
  if (m_attrShadowMats)
  {
    m_attrShadowMats->Set(m_shadowProjView, 6);
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
void vkPointLightvkGraphicsGL4::RenderShadow(vkEntity *root, const vkPointLight *light)
{
  CalcCubeMatrices(light);

  CalcShadowIntensity(light);

  vkScanConfig config;
  config.ScanShadowCasters = true;
  config.ScanNonShadowCasters = false;
  // collect the shadow casting objects
  vkDefaultCollector collector(&m_renderStates, &m_renderStates, &m_renderStates, 0, 0);
  m_renderStates.Clear();
  root->Scan(0, m_renderer, &collector, config);

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
  m_renderer->SetShadowMatrices(m_shadowProjView, m_shadowProj, m_shadowCam, m_shadowNearFar, 6);
  m_renderer->SetBlendEnabled(false);

  // render all geometries
  for (vkSize i = 0; i < m_renderStates.length; ++i)
  {
    vkRenderState *renderState = m_renderStates[i];
    if (renderState)
    {
      renderState->Render(m_renderer, eRP_ShadowCube);
    }
  }
  glColorMask(true, true, true, true);

}

void vkPointLightvkGraphicsGL4::CalcCubeMatrices(const vkPointLight *light)
{
  float radius = light->GetRadius();

  vkMatrix4f proj;
  m_renderer->GetPerspectiveProjection(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, radius, proj);

  vkVector3f spot;
  vkVector3f dirs[] = {
    vkVector3f(1, 0, 0),
    vkVector3f(-1, 0, 0),
    vkVector3f(0, 1, 0),
    vkVector3f(0, -1, 0),
    vkVector3f(0, 0, 1),
    vkVector3f(0, 0, -1),
  };
  vkVector3f ups[] = {
    vkVector3f(0, 0, 1),
    vkVector3f(0, 0, 1),
    vkVector3f(0, 0, 1),
    vkVector3f(0, 0, 1),
    vkVector3f(1, 0, 0),
    vkVector3f(-1, 0, 0),
  };

  for (int i = 0; i < 6; ++i)
  {
    vkVector3f::Add(light->GetPosition(), dirs[i], spot);
    m_shadowCam[i].SetLookAt(light->GetPosition(), spot, ups[i]);
    m_shadowProj[i] = proj;
    m_shadowNearFar[i].Set(1.0f, radius);
    vkMatrix4f::Mult(m_shadowProj[i], m_shadowCam[i], m_shadowProjView[i]);
  }

}

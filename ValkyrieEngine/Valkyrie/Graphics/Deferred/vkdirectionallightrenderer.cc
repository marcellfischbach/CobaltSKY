
#include <Valkyrie/Graphics/Deferred/vkdirectionallightrenderer.hh>
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



vkDirectionalLightRenderer::vkDirectionalLightRenderer(IGraphics *renderer)
  : vkLightRenderer(renderer)
  , m_colorBuffer(0)
  , m_colorBufferBlur(0)
  , m_depthBuffer(0)
{
  InitializeLightProgram(&m_programNoShadow, vkResourceLocator("${shaders}/deferred/DirectionalLight.xasset"));
  m_attrLightDirectionNoShadow = m_programNoShadow.program->GetAttribute(vkShaderAttributeID("LightDirection"));

  InitializeLightProgram(&m_programPSSM, vkResourceLocator("${shaders}/deferred/DirectionalLightPSSM.xasset"));
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
  m_colorBuffer = renderer->CreateTexture2DArray(shadowBufferFormat, (vkUInt16)m_shadowBufferSize, (vkUInt16)m_shadowBufferSize, 3, false);
  m_depthBuffer = renderer->CreateTexture2DArray(ePF_D24S8, (vkUInt16)m_shadowBufferSize, (vkUInt16)m_shadowBufferSize, 3, false);

  ISampler *colorSampler = renderer->CreateSampler();
  colorSampler->SetFilter(eFM_MinMagLinear);
  colorSampler->SetAddressU(eTAM_ClampBorder);
  colorSampler->SetAddressV(eTAM_ClampBorder);
  colorSampler->SetAddressW(eTAM_ClampBorder);

  m_colorBuffer->SetSampler(colorSampler);
  m_depthBuffer->SetSampler(m_depthSampler);

  colorSampler->Release();

  m_shadowBuffer = static_cast<IRenderTarget*>(renderer->CreateRenderTarget());
  m_shadowBuffer->Initialize((vkUInt16)m_shadowBufferSize, (vkUInt16)m_shadowBufferSize);
  m_shadowBuffer->AddColorTexture(m_colorBuffer);
  m_shadowBuffer->SetDepthTexture(m_depthBuffer);
  m_shadowBuffer->Finilize();

}

vkDirectionalLightRenderer::~vkDirectionalLightRenderer()
{
  VK_RELEASE(m_colorBuffer);
  VK_RELEASE(m_depthBuffer);
  VK_RELEASE(m_colorBufferBlur);
}




void vkDirectionalLightRenderer::Render(vkEntity *root, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target)
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
  m_renderer->SetRenderDestination(eRD_Color0);

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


void vkDirectionalLightRenderer::BindDirectionalLightNoShadow(vkDirectionalLight *directionalLight)
{
  if (m_attrLightDirectionNoShadow)
  {
    m_attrLightDirectionNoShadow->Set(directionalLight->GetDirection());
  }

}



void vkDirectionalLightRenderer::BindDirectionalLightPSSM(vkDirectionalLight *directionalLight)
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



void vkDirectionalLightRenderer::UpdateProjectionMatrices()
{
  float min = FLT_MAX;
  float max = -FLT_MAX;
  const vkMatrix4f &view = m_shadowCamAll;
  for (vkSize i = 0; i < eRQ_COUNT; ++i)
  {
    vkCollection<vkRenderState*> &queue = m_renderStates[i];
    for (vkSize j = 0; j < queue.length; ++j)
    {
      vkRenderState *renderState = queue[j];
      if (renderState)
      {
        const vkBoundingBox &bbox = renderState->GetBoundingBox();
        for (unsigned k = 0; k < 8; ++k)
        {
          vkVector3f p;
          vkMatrix4f::Transform(view, bbox.GetPoints()[k], p);
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
  }
  for (unsigned i = 0; i < 3; ++i)
  {
    m_min[i].y = min;
    m_shadowNearFar[i].Set(m_min[i].y, m_max[i].y);
    m_renderer->GetOrthographicProjection(m_min[i].x, m_max[i].x, m_min[i].z, m_max[i].z, m_min[i].y, m_max[i].y, m_shadowProj[i]);
    vkMatrix4f::Mult(m_shadowProj[i], m_shadowCam[i], m_shadowProjView[i]);

  }
}

vkClipper *vkDirectionalLightRenderer::CreateClipper()
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

void vkDirectionalLightRenderer::RenderShadow(vkEntity *root, vkCamera *camera, const vkDirectionalLight *light)
{
  CalcPSSMMatrices(light, camera);

  CalcShadowIntensity(light);

  vkScanConfig config;
  config.ScanShadowCasters = true;
  config.ScanNonShadowCasters = false;
  config.MainCameraPosition = camera->GetEye();
  // collect the shadow casting objects
  vkDefaultCollector collector(m_renderStates, 0);
  for (vkSize i = 0; i < eRQ_COUNT; ++i)
  {
    m_renderStates[i].Clear();
  }

  vkClipper *clipper = CreateClipper();
  root->Scan(clipper, m_renderer, &collector, config);
  delete clipper;


  UpdateProjectionMatrices();

  // setup the rendering 
  m_renderer->SetRenderTarget(m_shadowBuffer);
  m_renderer->SetViewport(m_shadowBuffer);
  m_renderer->SetRenderDestination(eRD_Color0);
  m_renderer->SetDepthMask(true);
  m_renderer->SetDepthTest(true);
  m_renderer->SetDepthFunc(eCM_LessOrEqual);
  m_renderer->SetColorMask(true, true, true, true);


  m_renderer->Clear(true, vkVector4f(m_max[2].y, m_max[2].y * m_max[2].y, 1, 1));
  m_renderer->SetShadowMatrices(m_shadowProjView, m_shadowProj, m_shadowCam, m_shadowNearFar, 3);
  m_renderer->SetBlendEnabled(false);

  for (vkSize i = 0; i < eRQ_COUNT; ++i)
  {
    vkCollection<vkRenderState*> &queue = m_renderStates[i];
    for (vkSize j = 0; j < queue.length; ++j)
    {
      vkRenderState *renderState = queue[j];
      if (renderState)
      {
        renderState->Render(m_renderer, eRP_ShadowPSSM);
      }
    }
  }
  m_renderer->SetColorMask(true, true, true, true);

}

void vkDirectionalLightRenderer::CalcPSSMMatrices(const vkDirectionalLight *light, const vkCamera *camera)
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

void vkDirectionalLightRenderer::CalcMatrix(const vkVector3f &dir, vkSize numPoints, vkVector3f *points, vkMatrix4f &cam, vkMatrix4f &camInv, vkVector3f &min, vkVector3f &max) const
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


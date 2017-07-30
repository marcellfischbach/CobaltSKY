
#include <cobalt/graphics/deferred/csdirectionallightrenderer.hh>
#include <cobalt/graphics/deferred/csdefaultcollector.hh>
#include <cobalt/graphics/deferred/csgbuffer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/irendertarget.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/graphics/itexture2darray.hh>
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/graphics/csdirectionallight.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/cssettings.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csgeometrydata.hh>
#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/math/csclipper.hh>



csDirectionalLightRenderer::csDirectionalLightRenderer(iGraphics *renderer)
  : csLightRenderer(renderer)
  , m_colorBuffer(0)
  , m_colorBufferBlur(0)
  , m_depthBuffer(0)
{
  InitializeLightProgram(&m_programNoShadow, csResourceLocator("${shaders}/deferred/DirectionalLight.xasset"));
  m_attrLightDirectionNoShadow = m_programNoShadow.program->GetAttribute(csShaderAttributeID("LightDirection"));

  InitializeLightProgram(&m_programPSSM, csResourceLocator("${shaders}/deferred/DirectionalLightPSSM.xasset"));
  m_attrLightDirectionPSSM = m_programPSSM.program->GetAttribute(csShaderAttributeID("LightDirection"));
  m_attrDisancesPSSM = m_programPSSM.program->GetAttribute(csShaderAttributeID("Distances"));
  m_attrShadowMatsProjView = m_programPSSM.program->GetAttribute(csShaderAttributeID("ShadowMatsProjView"));
  m_attrShadowMatsProj = m_programPSSM.program->GetAttribute(csShaderAttributeID("ShadowMatsProj"));
  m_attrShadowMatsView = m_programPSSM.program->GetAttribute(csShaderAttributeID("ShadowMatsView"));
  m_attrShadowProjNearFar = m_programPSSM.program->GetAttribute(csShaderAttributeID("ShadowProjNearFar"));
  m_attrShadowMap = m_programPSSM.program->GetAttribute(csShaderAttributeID("ShadowMap"));
  m_attrShadowColorMap = m_programPSSM.program->GetAttribute(csShaderAttributeID("ShadowColorMap"));
  m_attrShadowMapSizeInv = m_programPSSM.program->GetAttribute(csShaderAttributeID("ShadowMapSizeInv"));
  m_attrMapBias = m_programPSSM.program->GetAttribute(csShaderAttributeID("MapBias"));
  m_attrShadowIntensity = m_programPSSM.program->GetAttribute(csShaderAttributeID("ShadowIntensity"));



  m_distances.x = csSettings::Get()->GetFloatValue("graphicsGL4", "pssm.distance1", 5.0f);
  m_distances.y = csSettings::Get()->GetFloatValue("graphicsGL4", "pssm.distance2", 25.0f);
  m_distances.z = csSettings::Get()->GetFloatValue("graphicsGL4", "pssm.distance3", 120.0f);
  m_shadowBufferSize = csSettings::Get()->GetIntValue("graphicsGL4", "pssm.bufferSize", 1024);

  csPixelFormat shadowBufferFormat = ePF_R16G16F;
  m_colorBuffer = renderer->CreateTexture2DArray(shadowBufferFormat, (csUInt16)m_shadowBufferSize, (csUInt16)m_shadowBufferSize, 3, false);
  m_depthBuffer = renderer->CreateTexture2DArray(ePF_D24S8, (csUInt16)m_shadowBufferSize, (csUInt16)m_shadowBufferSize, 3, false);

  iSampler *colorSampler = renderer->CreateSampler();
  colorSampler->SetFilter(eFM_MinMagLinear);
  colorSampler->SetAddressU(eTAM_Clamp);
  colorSampler->SetAddressV(eTAM_Clamp);
  colorSampler->SetAddressW(eTAM_Clamp);

  m_colorBuffer->SetSampler(colorSampler);
  m_depthBuffer->SetSampler(m_depthSampler);

  colorSampler->Release();

  m_shadowBuffer = static_cast<iRenderTarget*>(renderer->CreateRenderTarget());
  m_shadowBuffer->Initialize((csUInt16)m_shadowBufferSize, (csUInt16)m_shadowBufferSize);
  m_shadowBuffer->AddColorTexture(m_colorBuffer);
  m_shadowBuffer->SetDepthTexture(m_depthBuffer);
  m_shadowBuffer->Finilize();

}

csDirectionalLightRenderer::~csDirectionalLightRenderer()
{
  CS_RELEASE(m_colorBuffer);
  CS_RELEASE(m_depthBuffer);
  CS_RELEASE(m_colorBufferBlur);
}




void csDirectionalLightRenderer::Render(csEntity *root, csCamera *camera, csLight *light, csGBuffer *gbuffer, iRenderTarget *target)
{
  csBlendMode blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha;
  m_renderer->GetBlendMode(blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha);
  bool blendEnabled = m_renderer->IsBlendEnabled();

  csDirectionalLight *directionalLight = static_cast<csDirectionalLight*>(light);

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


void csDirectionalLightRenderer::BindDirectionalLightNoShadow(csDirectionalLight *directionalLight)
{
  if (m_attrLightDirectionNoShadow)
  {
    m_attrLightDirectionNoShadow->Set(directionalLight->GetDirection());
  }

}



void csDirectionalLightRenderer::BindDirectionalLightPSSM(csDirectionalLight *directionalLight)
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
    csTextureUnit tu = m_renderer->BindTexture(m_depthBuffer);
    m_attrShadowMap->Set(tu);
  }
  if (m_attrShadowColorMap)
  {
    csTextureUnit tu = m_renderer->BindTexture(m_colorBuffer);
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
    csVector2f nearFar[3];
    nearFar[0] = csVector2f(m_min[0].y, m_max[0].y);
    nearFar[1] = csVector2f(m_min[1].y, m_max[1].y);
    nearFar[2] = csVector2f(m_min[2].y, m_max[2].y);
    m_attrShadowProjNearFar->Set(nearFar, 3);
  }
  if (m_attrShadowMapSizeInv)
  {
    m_attrShadowMapSizeInv->Set(1.0f / m_shadowBufferSize);
  }

}



void csDirectionalLightRenderer::UpdateProjectionMatrices()
{
  float min = FLT_MAX;
  float max = -FLT_MAX;
  const csMatrix4f &view = m_shadowCamAll;
  for (csSize i = 0; i < eRQ_COUNT; ++i)
  {
    csCollection<csRenderState*> &queue = m_renderStates[i];
    for (csSize j = 0; j < queue.length; ++j)
    {
      csRenderState *renderState = queue[j];
      if (renderState)
      {
        const csBoundingBox &bbox = renderState->GetBoundingBox();
        for (unsigned k = 0; k < 8; ++k)
        {
          csVector3f p;
          csMatrix4f::Transform(view, bbox.GetPoints()[k], p);
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
    csMatrix4f::Mult(m_shadowProj[i], m_shadowCam[i], m_shadowProjView[i]);

  }
}

csClipper *csDirectionalLightRenderer::CreateClipper()
{
  csVector3f topLeft(m_min[2].x, 0.0, m_max[2].z);
  csVector3f topRight(m_max[2].x, 0.0, m_max[2].z);
  csVector3f bottomLeft(m_min[2].x, 0.0, m_min[2].z);
  csVector3f bottomRight(m_max[2].x, 0.0, m_min[2].z);

  const csMatrix4f &camInv = m_shadowCamInv[2];
  csVector3f tl, tr, bl, br;
  csMatrix4f::Transform(camInv, topLeft, tl);
  csMatrix4f::Transform(camInv, topRight, tr);
  csMatrix4f::Transform(camInv, bottomLeft, bl);
  csMatrix4f::Transform(camInv, bottomRight, br);

  csVector3f r;
  camInv.GetXAxis(r);
  csVector3f l(r);
  l *= -1.0f;

  csVector3f t;
  camInv.GetZAxis(t);
  csVector3f b(t);
  b *= -1.0f;


  csPlaneClipper *clipper = new csPlaneClipper();
  clipper->AddPlane(csPlane(tl, r));
  clipper->AddPlane(csPlane(tl, b));
  clipper->AddPlane(csPlane(br, l));
  clipper->AddPlane(csPlane(br, t));
  return clipper;
}

void csDirectionalLightRenderer::RenderShadow(csEntity *root, csCamera *camera, const csDirectionalLight *light)
{
  CalcPSSMMatrices(light, camera);

  CalcShadowIntensity(light);

  csScanConfig config;
  config.ScanShadowCasters = true;
  config.ScanNonShadowCasters = false;
  config.MainCameraPosition = camera->GetEye();
  // collect the shadow casting objects
  csDefaultCollector collector(m_renderStates, 0);
  for (csSize i = 0; i < eRQ_COUNT; ++i)
  {
    m_renderStates[i].Clear();
  }

  csClipper *clipper = CreateClipper();
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

  csFaceSide current = m_renderer->GetCullFace();

  m_renderer->Clear(true, csVector4f(m_max[2].y, m_max[2].y * m_max[2].y, 1, 1));
  m_renderer->SetShadowMatrices(m_shadowProjView, m_shadowProj, m_shadowCam, m_shadowNearFar, 3);
  m_renderer->SetBlendEnabled(false);

  for (csSize i = 0; i < eRQ_COUNT; ++i)
  {
    csCollection<csRenderState*> &queue = m_renderStates[i];
    for (csSize j = 0; j < queue.length; ++j)
    {
      csRenderState *renderState = queue[j];
      if (renderState)
      {
        renderState->Render(m_renderer, eRP_ShadowPSSM);
      }
    }
  }
  m_renderer->SetCullFace(eFS_Back);
  m_renderer->SetColorMask(true, true, true, true);

}

void csDirectionalLightRenderer::CalcPSSMMatrices(const csDirectionalLight *light, const csCamera *camera)
{
  float dists[] = { 0.0f, m_distances.x, m_distances.y, m_distances.z };
  csVector3f points[8];

  for (csSize i = 0; i < 3; ++i)
  {
    camera->GetPlanePoints(0, &points[0]);
    camera->GetPlanePoints(dists[i + 1], &points[4]);
    CalcMatrix(light->GetDirection(), 8, points, m_shadowCam[i], m_shadowCamInv[i], m_min[i], m_max[i]);
  }

  camera->GetPlanePoints(0, &points[0]);
  camera->GetPlanePoints(m_distances.z, &points[4]);
  CalcMatrix(light->GetDirection(), 8, points, m_shadowCamAll, m_shadowCamInvAll, m_minAll, m_maxAll);
}

void csDirectionalLightRenderer::CalcMatrix(const csVector3f &dir, csSize numPoints, csVector3f *points, csMatrix4f &cam, csMatrix4f &camInv, csVector3f &min, csVector3f &max) const
{
  csVector3f spot;
  for (csSize i = 0; i < numPoints; i++)
  {
    csVector3f::Add(spot, points[i], spot);
  }
  csVector3f::Div(spot, (float)numPoints, spot);

  csVector3f eye;
  csVector3f::Mul(dir, -100.0f, eye);
  csVector3f::Add(eye, spot, eye);

  csVector3f up(0.0f, 0.0f, 1.0f);
  if (dir.z >= 0.9999999)
  {
    up.Set(1.0f, 0.0f, 0.0f);
  }
  else if (dir.z <= -0.9999999)
  {
    up.Set(-1.0f, 0.0f, 0.0f);
  }

  cam.SetLookAt(csVector3f(0, 0, 0), dir, up);
  camInv.SetLookAtInv(csVector3f(0, 0, 0), dir, up);

  min.Set(FLT_MAX, FLT_MAX, FLT_MAX);
  max.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
  csVector3f t;
  for (csSize i = 0; i < numPoints; i++)
  {
    csVector3f &p = points[i];
    csMatrix4f::Transform(cam, p, t);
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


#include <cobalt/graphics/deferred/cspointlightrenderer.hh>
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
#include <cobalt/graphics/cspointlight.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csgeometrydata.hh>
#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/math/csclipper.hh>



csPointLightRenderer::csPointLightRenderer(iGraphics *renderer)
  : csLightRenderer(renderer)
{
  InitializeLightProgram(&m_programNoShadow, csResourceLocator("${shaders}/deferred/PointLight.xasset"));
  m_attrLightPositionNoShadow = m_programNoShadow.program->GetAttribute(csShaderAttributeID("LightPosition"));
  m_attrLightRangeNoShadow = m_programNoShadow.program->GetAttribute(csShaderAttributeID("LightRadius"));

  InitializeLightProgram(&m_programCubeShadow, csResourceLocator("${shaders}/deferred/PointLightCubeShadow.xasset"));
  m_attrLightPositionCubeShadow = m_programCubeShadow.program->GetAttribute(csShaderAttributeID("LightPosition"));
  m_attrLightRangeCubeShadow = m_programCubeShadow.program->GetAttribute(csShaderAttributeID("LightRadius"));
  m_attrShadowMats = m_programCubeShadow.program->GetAttribute(csShaderAttributeID("ShadowMats"));
  m_attrShadowMap = m_programCubeShadow.program->GetAttribute(csShaderAttributeID("ShadowMap"));
  m_attrMapBias = m_programCubeShadow.program->GetAttribute(csShaderAttributeID("MapBias"));
  m_attrShadowIntensity = m_programCubeShadow.program->GetAttribute(csShaderAttributeID("ShadowIntensity"));


  m_mapBias = 0.99f;

  csUInt16 bufferSize = 1024;
  iTexture2DArray *colorBuffer = renderer->CreateTexture2DArray(ePF_RGBA, bufferSize, bufferSize, 6, false);
  m_depthBuffer = renderer->CreateTexture2DArray(ePF_D24S8, bufferSize, bufferSize, 6, false);

  colorBuffer->SetSampler(csGBuffer::GetColorSampler(renderer));
  m_depthBuffer->SetSampler(m_depthSampler);

  m_shadowBuffer = static_cast<iRenderTarget*>(renderer->CreateRenderTarget());
  m_shadowBuffer->Initialize(bufferSize, bufferSize);
  m_shadowBuffer->AddColorTexture(colorBuffer);
  m_shadowBuffer->SetDepthTexture(m_depthBuffer);
  m_shadowBuffer->Finilize();

  colorBuffer->Release();


}

csPointLightRenderer::~csPointLightRenderer()
{
  CS_RELEASE(m_depthBuffer);
}




void csPointLightRenderer::Render(csEntity *root, csCamera *camera, csLight *light, csGBuffer *gbuffer, iRenderTarget *target)
{
  csBlendMode blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha;
  m_renderer->GetBlendMode(blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha);
  bool blendEnabled = m_renderer->IsBlendEnabled();

  csPointLight *pointLight = static_cast<csPointLight*>(light);


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
  m_renderer->SetRenderDestination(eRD_Color0);

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





void csPointLightRenderer::BindPointLightNo(csPointLight *pointLight)
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


void csPointLightRenderer::BindPointLightCubeShadow(csPointLight *pointLight)
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
    csTextureUnit tu = m_renderer->BindTexture(m_depthBuffer);
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
void csPointLightRenderer::RenderShadow(csEntity *root, const csPointLight *light)
{
  CalcCubeMatrices(light);

  CalcShadowIntensity(light);

  csScanConfig config;
  config.ScanShadowCasters = true;
  config.ScanNonShadowCasters = false;
  // collect the shadow casting objects
  csDefaultCollector collector(m_renderStates, 0);
  for (csSize i = 0; i < eRQ_COUNT; ++i)
  {
    m_renderStates[i].Clear();
  }
  root->Scan(0, m_renderer, &collector, config);

  // setup the rendering 
  m_renderer->SetRenderTarget(m_shadowBuffer);
  m_renderer->SetViewport(m_shadowBuffer);
  m_renderer->SetRenderDestination(eRD_Color0);
  m_renderer->SetDepthMask(true);
  m_renderer->SetDepthTest(true);
  m_renderer->SetDepthFunc(eCM_LessOrEqual);
  m_renderer->SetColorMask(true, true, true, true);




  m_renderer->Clear();
  m_renderer->SetShadowMatrices(m_shadowProjView, m_shadowProj, m_shadowCam, m_shadowNearFar, 6);
  m_renderer->SetBlendEnabled(false);

  // render all geometries
  for (csSize i = 0; i < eRQ_COUNT; ++i)
  {
    csCollection<csRenderState*> &queue = m_renderStates[i];
    for (csSize j = 0; j < queue.length; ++j)
    {
      csRenderState *renderState = queue[j];
      if (renderState)
      {
        renderState->Render(m_renderer, eRP_ShadowCube);
      }
    }
  }
  m_renderer->SetColorMask(true, true, true, true);

}

void csPointLightRenderer::CalcCubeMatrices(const csPointLight *light)
{
  float radius = light->GetRadius();

  csMatrix4f proj;
  m_renderer->GetPerspectiveProjection(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, radius, proj);

  csVector3f spot;
  csVector3f dirs[] = {
    csVector3f(1, 0, 0),
    csVector3f(-1, 0, 0),
    csVector3f(0, 1, 0),
    csVector3f(0, -1, 0),
    csVector3f(0, 0, 1),
    csVector3f(0, 0, -1),
  };
  csVector3f ups[] = {
    csVector3f(0, 0, 1),
    csVector3f(0, 0, 1),
    csVector3f(0, 0, 1),
    csVector3f(0, 0, 1),
    csVector3f(1, 0, 0),
    csVector3f(-1, 0, 0),
  };

  for (int i = 0; i < 6; ++i)
  {
    csVector3f::Add(light->GetPosition(), dirs[i], spot);
    m_shadowCam[i].SetLookAt(light->GetPosition(), spot, ups[i]);
    m_shadowProj[i] = proj;
    m_shadowNearFar[i].Set(1.0f, radius);
    csMatrix4f::Mult(m_shadowProj[i], m_shadowCam[i], m_shadowProjView[i]);
  }

}

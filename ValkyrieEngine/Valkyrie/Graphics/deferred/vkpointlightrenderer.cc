#include <Valkyrie/Graphics/deferred/vkpointlightrenderer.hh>
#include <Valkyrie/Graphics/deferred/vkdefaultcollector.hh>
#include <Valkyrie/Graphics/deferred/vkgbuffer.hh>
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



vkPointLightRenderer::vkPointLightRenderer(IGraphics *renderer)
  : vkLightRenderer(renderer)
{
  InitializeLightProgram(&m_programNoShadow, vkResourceLocator("${shaders}/deferred/PointLight.xasset"));
  m_attrLightPositionNoShadow = m_programNoShadow.program->GetAttribute(vkShaderAttributeID("LightPosition"));
  m_attrLightRangeNoShadow = m_programNoShadow.program->GetAttribute(vkShaderAttributeID("LightRadius"));

  InitializeLightProgram(&m_programCubeShadow, vkResourceLocator("${shaders}/deferred/PointLightCubeShadow.xasset"));
  m_attrLightPositionCubeShadow = m_programCubeShadow.program->GetAttribute(vkShaderAttributeID("LightPosition"));
  m_attrLightRangeCubeShadow = m_programCubeShadow.program->GetAttribute(vkShaderAttributeID("LightRadius"));
  m_attrShadowMats = m_programCubeShadow.program->GetAttribute(vkShaderAttributeID("ShadowMats"));
  m_attrShadowMap = m_programCubeShadow.program->GetAttribute(vkShaderAttributeID("ShadowMap"));
  m_attrMapBias = m_programCubeShadow.program->GetAttribute(vkShaderAttributeID("MapBias"));
  m_attrShadowIntensity = m_programCubeShadow.program->GetAttribute(vkShaderAttributeID("ShadowIntensity"));


  m_mapBias = 0.99f;

  vkUInt16 bufferSize = 1024;
  ITexture2DArray *colorBuffer = renderer->CreateTexture2DArray(ePF_RGBA, bufferSize, bufferSize, 6, false);
  m_depthBuffer = renderer->CreateTexture2DArray(ePF_D24S8, bufferSize, bufferSize, 6, false);

  colorBuffer->SetSampler(vkGBuffer::GetColorSampler(renderer));
  m_depthBuffer->SetSampler(m_depthSampler);

  m_shadowBuffer = static_cast<IRenderTarget*>(renderer->CreateRenderTarget());
  m_shadowBuffer->Initialize(bufferSize, bufferSize);
  m_shadowBuffer->AddColorTexture(colorBuffer);
  m_shadowBuffer->SetDepthTexture(m_depthBuffer);
  m_shadowBuffer->Finilize();

  colorBuffer->Release();


}

vkPointLightRenderer::~vkPointLightRenderer()
{
  VK_RELEASE(m_depthBuffer);
}




void vkPointLightRenderer::Render(vkEntity *root, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target)
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





void vkPointLightRenderer::BindPointLightNo(vkPointLight *pointLight)
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


void vkPointLightRenderer::BindPointLightCubeShadow(vkPointLight *pointLight)
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
void vkPointLightRenderer::RenderShadow(vkEntity *root, const vkPointLight *light)
{
  CalcCubeMatrices(light);

  CalcShadowIntensity(light);

  vkScanConfig config;
  config.ScanShadowCasters = true;
  config.ScanNonShadowCasters = false;
  // collect the shadow casting objects
  vkDefaultCollector collector(m_renderStates, 0);
  for (vkSize i = 0; i < eRQ_COUNT; ++i)
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
  for (vkSize i = 0; i < eRQ_COUNT; ++i)
  {
    vkCollection<vkRenderState*> &queue = m_renderStates[i];
    for (vkSize j = 0; j < queue.length; ++j)
    {
      vkRenderState *renderState = queue[j];
      if (renderState)
      {
        renderState->Render(m_renderer, eRP_ShadowCube);
      }
    }
  }
  m_renderer->SetColorMask(true, true, true, true);

}

void vkPointLightRenderer::CalcCubeMatrices(const vkPointLight *light)
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

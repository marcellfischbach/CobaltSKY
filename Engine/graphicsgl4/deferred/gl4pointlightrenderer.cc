#include <graphicsgl4/deferred/gl4pointlightrenderer.hh>
#include <graphicsgl4/deferred/gl4defaultcollector.hh>
#include <graphicsgl4/deferred/gl4gbuffer.hh>
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



cs::PointLightRendererGL4::PointLightRendererGL4(cs::iGraphics *renderer)
  : cs::LightRendererGL4(renderer)
{
  InitializeLightProgram(&m_programNoShadow, cs::ResourceLocator("${shaders}/deferred/PointLight.asset"));
  m_attrLightPositionNoShadow = m_programNoShadow.program->Get()->GetAttribute(cs::ShaderAttributeID("LightPosition"));
  m_attrLightRangeNoShadow = m_programNoShadow.program->Get()->GetAttribute(cs::ShaderAttributeID("LightRadius"));

  InitializeLightProgram(&m_programCubeShadow, cs::ResourceLocator("${shaders}/deferred/PointLightCubeShadow.asset"));
  m_attrLightPositionCubeShadow = m_programCubeShadow.program->Get()->GetAttribute(cs::ShaderAttributeID("LightPosition"));
  m_attrLightRangeCubeShadow = m_programCubeShadow.program->Get()->GetAttribute(cs::ShaderAttributeID("LightRadius"));
  m_attrShadowMats = m_programCubeShadow.program->Get()->GetAttribute(cs::ShaderAttributeID("ShadowMats"));
  m_attrShadowMap = m_programCubeShadow.program->Get()->GetAttribute(cs::ShaderAttributeID("ShadowMap"));
  m_attrMapBias = m_programCubeShadow.program->Get()->GetAttribute(cs::ShaderAttributeID("MapBias"));
  m_attrShadowIntensity = m_programCubeShadow.program->Get()->GetAttribute(cs::ShaderAttributeID("ShadowIntensity"));


  m_mapBias = 0.99f;

  csUInt16 bufferSize = 1024;
  cs::Texture2DArrayWrapper *colorBuffer = new cs::Texture2DArrayWrapper(renderer->CreateTexture2DArray(cs::ePF_RGBA, bufferSize, bufferSize, 6, false));
  m_depthBuffer = new cs::Texture2DArrayWrapper(renderer->CreateTexture2DArray(cs::ePF_D24S8, bufferSize, bufferSize, 6, false));

  colorBuffer->Get()->SetSampler(cs::GBufferGL4::GetColorSampler(renderer));
  m_depthBuffer->Get()->SetSampler(m_depthSampler);

  m_shadowBuffer = static_cast<cs::iRenderTarget*>(renderer->CreateRenderTarget());
  m_shadowBuffer->Initialize(bufferSize, bufferSize);
  m_shadowBuffer->AddColorTexture(colorBuffer);
  m_shadowBuffer->SetDepthTexture(m_depthBuffer);
  m_shadowBuffer->Finilize();

  colorBuffer->Release();


}

cs::PointLightRendererGL4::~PointLightRendererGL4()
{
  CS_RELEASE(m_depthBuffer);
}




void cs::PointLightRendererGL4::Render(cs::Entity *root, cs::Camera *camera, cs::Light *light, cs::GBufferGL4 *gbuffer, cs::iRenderTarget *target)
{
  cs::eBlendMode blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha;
  m_renderer->GetBlendMode(blendModeSrcColor, blendModeSrcAlpha, blendModeDstColor, blendModeDstAlpha);
  bool blendEnabled = m_renderer->IsBlendEnabled();

  cs::PointLight *pointLight = static_cast<cs::PointLight*>(light);


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
  m_renderer->SetRenderDestination(cs::eRD_Color0);

  LightProgram &prog = shadow ? m_programCubeShadow : m_programNoShadow;
  m_renderer->SetShader(prog.program->Get());
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





void cs::PointLightRendererGL4::BindPointLightNo(cs::PointLight *pointLight)
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


void cs::PointLightRendererGL4::BindPointLightCubeShadow(cs::PointLight *pointLight)
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
    cs::eTextureUnit tu = m_renderer->BindTexture(m_depthBuffer->Get());
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
void cs::PointLightRendererGL4::RenderShadow(cs::Entity *root, const cs::PointLight *light)
{
  m_renderer->PushRenderStates();

  CalcCubeMatrices(light);

  CalcShadowIntensity(light);

  cs::ScanConfig config;
  config.ScanShadowCasters = true;
  config.ScanNonShadowCasters = false;
  // collect the shadow casting objects
  cs::DefaultCollectorGL4 collector(m_renderStates, 0);
  for (csSize i = 0; i < cs::eRQ_COUNT; ++i)
  {
    m_renderStates[i].Clear();
  }
  root->Scan(0, m_renderer, &collector, config);

  // setup the rendering 
  m_renderer->SetRenderTarget(m_shadowBuffer);
  m_renderer->SetViewport(m_shadowBuffer);
  m_renderer->SetRenderDestination(cs::eRD_Color0);
  m_renderer->SetDepthMask(true);
  m_renderer->SetDepthTest(true);
  m_renderer->SetDepthFunc(cs::eCM_LessOrEqual);
  m_renderer->SetColorMask(true, true, true, true);




  m_renderer->Clear();
  m_renderer->SetShadowMatrices(m_shadowProjView, m_shadowProj, m_shadowCam, m_shadowNearFar, 6);
  m_renderer->SetBlendEnabled(false);

  // render all geometries
  for (csSize i = 0; i < cs::eRQ_COUNT; ++i)
  {
    cs::Collection<cs::RenderState*> &queue = m_renderStates[i];
    for (csSize j = 0; j < queue.length; ++j)
    {
      cs::RenderState *renderState = queue[j];
      if (renderState)
      {
        renderState->Render(m_renderer, cs::eRP_ShadowCube);
      }
    }
  }
  m_renderer->SetColorMask(true, true, true, true);

  m_renderer->PopRenderStates();
}

void cs::PointLightRendererGL4::CalcCubeMatrices(const cs::PointLight *light)
{
  float radius = light->GetRadius();

  cs::Matrix4f proj;
  m_renderer->GetPerspectiveProjection(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, radius, proj);

  cs::Vector3f spot;
  cs::Vector3f dirs[] = {
    cs::Vector3f(1, 0, 0),
    cs::Vector3f(-1, 0, 0),
    cs::Vector3f(0, 1, 0),
    cs::Vector3f(0, -1, 0),
    cs::Vector3f(0, 0, 1),
    cs::Vector3f(0, 0, -1),
  };
  cs::Vector3f ups[] = {
    cs::Vector3f(0, 0, 1),
    cs::Vector3f(0, 0, 1),
    cs::Vector3f(0, 0, 1),
    cs::Vector3f(0, 0, 1),
    cs::Vector3f(1, 0, 0),
    cs::Vector3f(-1, 0, 0),
  };

  for (int i = 0; i < 6; ++i)
  {
    cs::Vector3f::Add(light->GetPosition(), dirs[i], spot);
    m_shadowCam[i].SetLookAt(light->GetPosition(), spot, ups[i]);
    m_shadowProj[i] = proj;
    m_shadowNearFar[i].Set(1.0f, radius);
    cs::Matrix4f::Mult(m_shadowProj[i], m_shadowCam[i], m_shadowProjView[i]);
  }

}

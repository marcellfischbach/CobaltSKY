
#include <Valkyrie/Graphics/Deferred/DeferredFrameProcessor.hh>
#include <Valkyrie/Core/vkcollection.hh>
#include <Valkyrie/Core/vkresourcemanager.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/Geometry.hh>
#include <Valkyrie/Entity/LightState.hh>
#include <Valkyrie/Entity/RenderState.hh>
#include <Valkyrie/Graphics/Camera.hh>
#include <Valkyrie/Graphics/Light.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Graphics/PostProcessing.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Graphics/Deferred/GBuffer.hh>
#include <Valkyrie/Graphics/Deferred/LightRenderer.hh>
#include <Valkyrie/Graphics/Deferred/ParticleRenderer.hh>
#include <algorithm>

vkDeferredFrameProcessor::vkDeferredFrameProcessor(IGraphics *renderer)
  : IFrameProcessor()
  , m_renderer(renderer)
  , m_gbuffer(0)
  , m_postProcessor(0)
{
  VK_CLASS_GEN_CONSTR;

  for (unsigned i = 0; i < eRQ_COUNT; ++i)
  {
    m_renderStates[i].Initialize(64, 16);
  }
  m_lightRenderers[eLT_DirectionalLight] = new vkDirectionalLightRenderer(renderer);
  m_lightRenderers[eLT_PointLight] = new vkPointLightRenderer(renderer);

  m_particleRenderer = new vkParticleRenderer();


  // glGenQueries(10, queries);
}

vkDeferredFrameProcessor::~vkDeferredFrameProcessor()
{
  delete m_lightRenderers[eLT_DirectionalLight];
  delete m_lightRenderers[eLT_PointLight];

  VK_RELEASE(m_simplePresentShader);
  VK_RELEASE(m_directionLightShader);
  VK_RELEASE(m_gbuffer);
  VK_RELEASE(m_postProcessor);
  //glDeleteQueries(10, queries);
}

bool vkDeferredFrameProcessor::Initialize()
{

  vkResourceManager *mgr = vkResourceManager::Get();

  m_simplePresentShader = mgr->GetOrLoad<IShader>(vkResourceLocator("${shaders}/deferred/SimplePresent.xasset"));
  if (!m_simplePresentShader)
  {
    return false;
  }
  m_simplePresentShader->AddRef();

  m_directionLightShader = mgr->GetOrLoad<IShader>(vkResourceLocator("${shaders}/deferred/DirectionalLight.xasset"));
  if (!m_directionLightShader)
  {
    return false;
  }
  m_directionLightShader->AddRef();

  return true;
}

bool vkDeferredFrameProcessor::Resize(vkUInt16 width, vkUInt16 height)
{
  if (!m_gbuffer)
  {
    m_gbuffer = new vkGBuffer(m_renderer);
  }
  if (!m_gbuffer->Resize(width, height))
  {
    m_gbuffer->Release();
    m_gbuffer = 0;
    printf("Unable to resize\n");
    return false;
  }
  return true;
}

void vkDeferredFrameProcessor::SetPostProcessor(vkPostProcessor *postProcessor)
{
  VK_SET(m_postProcessor, postProcessor);
}


struct Data
{
  const vkMatrix4f *matrix;
  vkMaterialInstance *material;
  vkSubMesh *mesh;
  Data(const vkMatrix4f *matrix, vkMaterialInstance *material, vkSubMesh *mesh)
    : matrix(matrix)
    , material(material)
    , mesh(mesh)
  {

  }

  bool operator< (const Data& o)
  {
    return material < o.material;
  }
};



void vkDeferredFrameProcessor::RenderGBuffer(vkEntity *root)
{

  // render the scene to the GBuffer
  if (!m_gbuffer->Bind(m_renderer))
  {
    return;
  }

  m_renderer->SetDepthMask(true);
  m_renderer->SetDepthTest(true);
  m_renderer->SetDepthFunc(eCM_LessOrEqual);

//  glDepthMask(true);
//  glEnable(GL_DEPTH_TEST);
//  glDepthFunc(GL_LEQUAL);
//  glClearDepth(1.0);


  vkRenderDestination destinations[] = {
    eRD_Color0,
    eRD_Color1,
    eRD_Color2,
    eRD_Color3,
  };
  m_renderer->SetRenderDestinations(destinations, 4);
  m_renderer->Clear();
  m_renderer->SetBlendEnabled(false);

  for (vkSize i = 0; i < m_renderStates[eRQ_Deferred].length; ++i)
  {
    vkRenderState *renderState = m_renderStates[eRQ_Deferred][i];
    if (renderState)
    {
      renderState->Render(m_renderer, eRP_GBuffer);
    }
  }
}

IRenderTarget *vkDeferredFrameProcessor::Render(vkEntity *root, vkCamera *camera, IRenderTarget *target)
{
  for (unsigned i = 0; i < eRQ_COUNT; i++)
  {
    m_renderStates[i].Clear();
  }
  m_lightStates.Clear();

  vkScanConfig config;
  config.ScanNonShadowCasters = true;
  config.ScanShadowCasters = true;
  config.MainCameraPosition = camera->GetEye();
  vkDefaultCollector collector(m_renderStates,
                               &m_lightStates);
  vkClipper *clipper = camera->GetClipper();
  if (root)
  {
    root->Scan(clipper, m_renderer, &collector, config);
  }

  camera->Apply(m_renderer);
  
  //glBeginQuery(GL_TIME_ELAPSED, queries[0]);
  // render to the main GBuffer this buffer will be used to assemble the final image
  if (root)
  {
    RenderGBuffer(root);
  }
  //glEndQuery(GL_TIME_ELAPSED);


  //glBeginQuery(GL_TIME_ELAPSED, queries[1]);
  m_renderer->SetRenderTarget(target);
  m_renderer->SetViewport(target);
  m_renderer->Clear(true, vkVector4f(0.0f, 0.0f, 0.0f, 0.0f), false, 1.0f, false, 0);


  m_renderer->SetBlendEnabled(true);
  m_renderer->SetBlendMode(eBM_One, eBM_One);

  for (vkSize i = 0; i < m_lightStates.length; ++i)
  {
    vkLightState *lightState = m_lightStates[i];
    if (!lightState)
    {
      continue;
    }
    vkLight* light = lightState->GetLight();

    vkLightRenderer *lightRenderer = m_lightRenderers[light->GetLightType()];
    if (lightRenderer)
    {
      lightRenderer->Render(root, camera, light, m_gbuffer, target);
    }
  }

  //glEndQuery(GL_TIME_ELAPSED);

  target->SetDepthTexture(m_gbuffer->GetDepth());
  camera->Apply(m_renderer);
  m_renderer->SetBlendEnabled(false);
  m_renderer->SetDepthMask(true);
  m_renderer->SetDepthTest(true);
  m_renderer->SetDepthFunc(eCM_LessOrEqual);
  for (vkSize i=0; i<m_renderStates[eRQ_Forward].length; ++i)
  {
    vkRenderState *renderState = m_renderStates[eRQ_Forward][i];
    if (!renderState)
    {
      continue;
    }

    RenderForward(renderState);

  }

  m_particleRenderer->Render(m_renderer, m_renderStates[eRQ_Particles]);


#if 0
  GLuint time0, time1;
  glGetQueryObjectuiv(queries[0], GL_QUERY_RESULT, &time0);
  glGetQueryObjectuiv(queries[1], GL_QUERY_RESULT, &time1);
  printf("Times: %.2f %.2f\n", (float)(time0 / 1000) / 1000.0f, (float)(time1 / 1000.0f) / 1000.0f);
#endif
  m_renderer->SetBlendEnabled(false);

  if (m_postProcessor)
  {
    // rebind the camera because it might be switched by a light renderer
    camera->Apply(m_renderer);
    m_postProcessor->SetInput(vkPostProcessor::eOO_GBuffer_Depth, m_gbuffer->GetDepth());
    m_postProcessor->SetInput(vkPostProcessor::eOO_GBuffer_DiffuseRoughness, m_gbuffer->GetDiffuseRoughness());
    m_postProcessor->SetInput(vkPostProcessor::eOO_GBuffer_NormalLightMode, m_gbuffer->GetNormalLightMode());
    m_postProcessor->SetInput(vkPostProcessor::eOO_GBuffer_EmissiveMetallic, m_gbuffer->GetEmissiveMetallic());
    m_postProcessor->SetInput(vkPostProcessor::eOO_GBuffer_SSSSpec, m_gbuffer->GetSSSSpec());
    m_postProcessor->SetInput(vkPostProcessor::eOO_FinalTarget_Color, target->GetColorBuffer(0));
    m_postProcessor->SetInput(vkPostProcessor::eOO_FinalTarget_Depth, target->GetDepthBuffer());
    m_postProcessor->Render(m_renderer);
  }

  ITexture2DArray *txt = vkQueryClass<ITexture2DArray>(m_lightRenderers[eLT_PointLight]->GetShadowBuffer()->GetColorBuffer(0));
  //m_renderer->RenderFullScreenFrame(m_gbuffer->GetDiffuseRoughness());
  //m_renderer->RenderFullScreenFrame(0.0f, 0.25, 0.0, 0.25, txt, 0);
  //m_renderer->RenderFullScreenFrame(0.25f, 0.5, 0.0, 0.25, txt, 3);
  //m_renderer->RenderFullScreenFrame(0.5f, 0.75, 0.0f, 0.25f, txt, 1);
  //m_renderer->RenderFullScreenFrame(0.75f, 1.0, 0.0f, 0.25f, txt, 2);


  //m_renderer->RenderFullScreenFrame(txt, 1);
  
  m_renderer->SetBlendEnabled(false);
  if (m_postProcessor)
  {
    return m_postProcessor->GetOutput();
  }
  return target;
}

void vkDeferredFrameProcessor::RenderForward(vkRenderState *renderState)
{
  renderState->Render(m_renderer, eRP_ForwardUnlit);
}

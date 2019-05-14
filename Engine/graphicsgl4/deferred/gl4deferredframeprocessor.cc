
#include <graphicsgl4/deferred/gl4deferredframeprocessor.hh>
#include <cobalt/core/cscollection.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csgeometrydata.hh>
#include <cobalt/entity/cslightstate.hh>
#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/graphics/cscamera.hh>
#include <cobalt/graphics/cslight.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/cspostprocess.hh>
#include <cobalt/graphics/cspostprocessoutput.hh>
#include <cobalt/graphics/cspostprocessor.hh>
#include <cobalt/graphics/irendertarget.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/itexture.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/graphics/itexture2darray.hh>
#include <graphicsgl4/deferred/gl4directionallightrenderer.hh>
#include <graphicsgl4/deferred/gl4gbuffer.hh>
#include <graphicsgl4/deferred/gl4particlerenderer.hh>
#include <graphicsgl4/deferred/gl4pointlightrenderer.hh>
#include <cobalt/math/cscolor4f.hh>
#include <algorithm>

csDeferredFrameProcessorGL4::csDeferredFrameProcessorGL4(iGraphics *renderer)
  : iFrameProcessor()
  , m_frameNo(1)
  , m_renderer(renderer)
  , m_gbuffer(0)
  , m_postProcessor(0)
  , m_clearColor(0.0f, 0.0f, 0.0f, 1.0f)
{
  CS_CLASS_GEN_CONSTR;

  for (unsigned i = 0; i < eRQ_COUNT; ++i)
  {
    m_renderStates[i].Initialize(64, 16);
  }
  m_lightRenderers[eLT_DirectionalLight] = new csDirectionalLightRendererGL4(renderer);
  m_lightRenderers[eLT_PointLight] = new csPointLightRendererGL4(renderer);

  m_particleRenderer = new csParticleRendererGL4();


  // glGenQueries(10, queries);
}

csDeferredFrameProcessorGL4::~csDeferredFrameProcessorGL4()
{
  delete m_lightRenderers[eLT_DirectionalLight];
  delete m_lightRenderers[eLT_PointLight];

  CS_RELEASE(m_simplePresentShader);
  CS_RELEASE(m_directionLightShader);
  CS_RELEASE(m_gbuffer);
  CS_RELEASE(m_postProcessor);
  //glDeleteQueries(10, queries);
}

bool csDeferredFrameProcessorGL4::Initialize()
{

  csResourceManager *mgr = csResourceManager::Get();

  m_simplePresentShader = mgr->GetOrLoad<iShader>(csResourceLocator("${shaders}/deferred/SimplePresent.asset"));
  if (!m_simplePresentShader)
  {
    return false;
  }
  m_simplePresentShader->AddRef();

  m_directionLightShader = mgr->GetOrLoad<iShader>(csResourceLocator("${shaders}/deferred/DirectionalLight.asset"));
  if (!m_directionLightShader)
  {
    return false;
  }
  m_directionLightShader->AddRef();

  return true;
}

bool csDeferredFrameProcessorGL4::Resize(csUInt16 width, csUInt16 height)
{
  if (!m_gbuffer)
  {
    m_gbuffer = new csGBufferGL4(m_renderer);
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

void csDeferredFrameProcessorGL4::SetPostProcessor(csPostProcessor *postProcessor)
{
  CS_SET(m_postProcessor, postProcessor);
}

void csDeferredFrameProcessorGL4::SetClearColor(const csColor4f &color)
{
  m_clearColor = color;
}

const csColor4f &csDeferredFrameProcessorGL4::GetClearColor() const
{
  return m_clearColor;
}

struct Data
{
  const csMatrix4f *matrix;
  csMaterial *material;
  csSubMeshWrapper *mesh;
  Data(const csMatrix4f *matrix, csMaterial *material, csSubMeshWrapper *mesh)
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



void csDeferredFrameProcessorGL4::RenderGBuffer(csEntity *root)
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


  csRenderDestination destinations[] = {
    eRD_Color0,
    eRD_Color1,
    eRD_Color2,
    eRD_Color3,
  };
  m_renderer->SetRenderDestinations(destinations, 4);
  m_renderer->Clear();
  m_renderer->SetBlendEnabled(false);

  unsigned cnt = 0;
  unsigned calls = 0;
  unsigned trigons = 0;
  m_renderer->PushRenderStates();
  for (csSize i = 0; i < m_renderStates[eRQ_Deferred].length; ++i)
  {
    csRenderState *renderState = m_renderStates[eRQ_Deferred][i];
    if (renderState)
    {
      renderState->Render(m_renderer, eRP_GBuffer);
      ++cnt;
      //calls += renderState->GetNumberOfRenderCalls();
      //trigons += renderState->GetNumberOfTotalTrigons();
    }
  }
  m_renderer->PopRenderStates();
  //printf("Count: States: %u   Calls: %u    Trigons: %u\n", cnt, calls, trigons);
}

iRenderTarget *csDeferredFrameProcessorGL4::Render(csEntity *root, csCamera *camera, iRenderTarget *target)
{
  for (unsigned i = 0; i < eRQ_COUNT; i++)
  {
    m_renderStates[i].Clear();
  }
  m_lightStates.Clear();


  csScanConfig config;
  config.FrameNo = m_frameNo++;
  config.ScanNonShadowCasters = true;
  config.ScanShadowCasters = true;
  config.MainCameraPosition = camera->GetEye();
  csDefaultCollectorGL4 collector(m_renderStates,
                               &m_lightStates);
  csClipper *clipper = camera->GetClipper();
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
  m_renderer->Clear(true, csVector4f(m_clearColor.r, m_clearColor.g, m_clearColor.g, m_clearColor.a), false, 1.0f, false, 0);


  m_renderer->SetBlendEnabled(true);
  m_renderer->SetBlendMode(eBM_One, eBM_One);

  for (csSize i = 0; i < m_lightStates.length; ++i)
  {
    csLightState *lightState = m_lightStates[i];
    if (!lightState)
    {
      continue;
    }
    csLight* light = lightState->GetLight();

    csLightRendererGL4 *lightRenderer = m_lightRenderers[light->GetLightType()];
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
  for (csSize i=0; i<m_renderStates[eRQ_Forward].length; ++i)
  {
    csRenderState *renderState = m_renderStates[eRQ_Forward][i];
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
    m_postProcessor->SetInput(ePPO_GBuffer_Depth, m_gbuffer->GetDepth());
    m_postProcessor->SetInput(ePPO_GBuffer_DiffuseRoughness, m_gbuffer->GetDiffuseRoughness());
    m_postProcessor->SetInput(ePPO_GBuffer_NormalLightMode, m_gbuffer->GetNormalLightMode());
    m_postProcessor->SetInput(ePPO_GBuffer_EmissiveMetallic, m_gbuffer->GetEmissiveMetallic());
    m_postProcessor->SetInput(ePPO_GBuffer_SSSSpec, m_gbuffer->GetSSSSpec());
    m_postProcessor->SetInput(ePPO_FinalTarget_Color, target->GetColorBuffer(0));
    m_postProcessor->SetInput(ePPO_FinalTarget_Depth, target->GetDepthBuffer());
    m_postProcessor->Render(m_renderer);
  }

  iTexture2DArray *txt = cs::QueryClass<iTexture2DArray>(m_lightRenderers[eLT_PointLight]->GetShadowBuffer()->GetColorBuffer(0));
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

void csDeferredFrameProcessorGL4::RenderForward(csRenderState *renderState)
{
  renderState->Render(m_renderer, eRP_ForwardUnlit);
}

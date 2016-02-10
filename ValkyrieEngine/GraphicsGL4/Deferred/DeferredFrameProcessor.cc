
#include <GraphicsGL4/Deferred/DeferredFrameProcessor.hh>
#include <GraphicsGL4/Deferred/LightRenderer.hh>
#include <GraphicsGL4/GraphicsGL4.hh>
#include <GraphicsGL4/RenderTargetGL4.hh>
#include <GraphicsGL4/TextureGL4.hh>
#include <GraphicsGL4/Deferred/GBuffer.hh>
#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Core/ResourceManager.hh>
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
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <GL/glew.h>
#include <algorithm>

GLuint queries[10];


vkDeferredFrameProcessor::vkDeferredFrameProcessor(vkGraphicsGL4 *renderer)
  : IFrameProcessor()
  , m_renderer(renderer)
  , m_renderStates(64, 16)
  , m_gbuffer(0)
  , m_postProcessor(0)
{
  VK_CLASS_GEN_CONSTR;

  m_lightRenderers[eLT_DirectionalLight] = new vkDirectionalLightvkGraphicsGL4(renderer);
  m_lightRenderers[eLT_PointLight] = new vkPointLightvkGraphicsGL4(renderer);
  glGenQueries(10, queries);
}

vkDeferredFrameProcessor::~vkDeferredFrameProcessor()
{

}

bool vkDeferredFrameProcessor::Initialize(vkUInt16 width, vkUInt16 height)
{
  m_gbuffer = new vkGBuffer(m_renderer, width, height);
  if (!m_gbuffer->IsValid())
  {
    return false;
  }

  vkResourceManager *mgr = vkResourceManager::Get();

  m_simplePresentShader = mgr->GetOrLoad<IShader>(vkResourceLocator("${shaders}/deferred/deferred.xml", "SimplePresent"));
  if (!m_simplePresentShader)
  {
    return false;
  }

  m_directionLightShader = mgr->GetOrLoad<IShader>(vkResourceLocator("${shaders}/deferred/deferred.xml", "DirectionalLight"));
  if (!m_directionLightShader)
  {
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

  glDepthMask(true);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0);


  m_renderer->Clear();
  m_renderer->SetBlendEnabled(false);

  GLenum buffers[] = {
    GL_COLOR_ATTACHMENT0, // Diffuse / Roughness
    GL_COLOR_ATTACHMENT1, // Normal / LightMode
    GL_COLOR_ATTACHMENT2, // Emission / Metallic
    GL_COLOR_ATTACHMENT3, // SSS / Specular
  };
  glDrawBuffers(4, buffers);

  //printf ("Num Meshes: %d\n", m_renderStates.length);
  for (vkSize i = 0; i < m_renderStates.length; ++i)
  {
    vkRenderState *renderState = m_renderStates[i];
    if (renderState)
    {
      renderState->Render(m_renderer, eRP_GBuffer);
    }
  }
}

IRenderTarget *vkDeferredFrameProcessor::Render(vkEntity *root, vkCamera *camera, IRenderTarget *target)
{
  m_renderStates.Clear();
  m_lightStates.Clear();

  vkScanConfig config;
  config.ScanNonShadowCasters = true;
  config.ScanShadowCasters = true;
  config.MainCameraPosition = camera->GetEye();
  vkDefaultCollector collector(&m_renderStates, &m_lightStates);
  vkClipper *clipper = camera->GetClipper();
  root->Scan(clipper, m_renderer, &collector, config);

  camera->Apply(m_renderer);

  glBeginQuery(GL_TIME_ELAPSED, queries[0]);
  // render to the main GBuffer this buffer will be used to assemble the final image
  RenderGBuffer(root);
  glEndQuery(GL_TIME_ELAPSED);


  glBeginQuery(GL_TIME_ELAPSED, queries[1]);
  m_renderer->SetRenderTarget(target);
  m_renderer->SetViewport(target);
  m_renderer->SetBlendEnabled(true);
  m_renderer->SetBlendMode(eBM_One, eBM_One);
  m_renderer->Clear(true, vkVector4f(0.0f, 0.0f, 0.0f, 0.0f), true, 1.0f, false, 0);

  for (vkSize i = 0; i < m_lightStates.length; ++i)
  {
    vkLightState *lightState = m_lightStates[i];
    if (!lightState)
    {
      continue;
    }
    vkLight* light = lightState->GetLight();

    vkLightvkGraphicsGL4 *lightRenderer = m_lightRenderers[light->GetLightType()];
    if (lightRenderer)
    {
      lightRenderer->Render(root, camera, light, m_gbuffer, target);
    }

  }

  glEndQuery(GL_TIME_ELAPSED);

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

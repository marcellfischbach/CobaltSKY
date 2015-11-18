
#include <RenderGL4/Deferred/DeferredFrameProcessor.hh>
#include <RenderGL4/Deferred/LightRenderer.hh>
#include <RenderGL4/RendererGL4.hh>
#include <RenderGL4/RenderTargetGL4.hh>
#include <RenderGL4/TextureGL4.hh>
#include <RenderGL4/Deferred/GBuffer.hh>
#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Graphics/Camera.hh>
#include <Valkyrie/Graphics/Light.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Graphics/Scene/Node.hh>
#include <Valkyrie/Graphics/Scene/GeometryNode.hh>
#include <Valkyrie/Graphics/Scene/LightNode.hh>
#include <GL/glew.h>


vkDeferredFrameProcessor::vkDeferredFrameProcessor(RendererGL4 *renderer)
  : IFrameProcessor()
  , m_renderer(renderer)
  , m_geometries(64, 16)
  , m_gbuffer(0)
{
  VK_CLASS_GEN_CONSTR;

  m_lightRenderers[eLT_DirectionalLight] = new vkDirectionalLightRendererGL4(renderer);
  m_lightRenderers[eLT_PointLight] = new vkPointLightRendererGL4(renderer);
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




void vkDeferredFrameProcessor::RenderGBuffer(vkNode *rootNode)
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


  for (vkSize i = 0; i < m_geometries.length; ++i)
  {
    vkGeometryNode *geometryNode = m_geometries[i];
    if (geometryNode)
    {
      geometryNode->Render(m_renderer, eRP_GBuffer);
    }
  }

}

void vkDeferredFrameProcessor::Render(vkNode *node, const vkCamera *camera, IRenderTarget *target)
{
  m_geometries.Clear();
  m_lights.Clear();
  vkDefaultCollector collector(&m_geometries, &m_lights);
  node->Scan(0, m_renderer, &collector);

  camera->Apply(m_renderer);

  // render to the main GBuffer this buffer will be used to assemble the final image
  RenderGBuffer(node);


  m_renderer->SetRenderTarget(target);
  m_renderer->SetViewport(target);
  m_renderer->SetBlendEnabled(true);
  m_renderer->SetBlendMode(eBM_One, eBM_One);
  m_renderer->Clear(true, vkVector4f(0.0f, 0.0f, 0.0f, 0.0f), true, 1.0f, false, 0);

  for (vkSize i = 0; i < m_lights.length; ++i)
  {
    vkLightNode *lightNode = m_lights[i];
    vkLight* light = lightNode->GetLight();

    vkLightRendererGL4 *lightRenderer = m_lightRenderers[light->GetLightType()];
    if (lightRenderer)
    {
      lightRenderer->Render(node, camera, light, m_gbuffer, target);
    }

  }
  m_renderer->SetBlendEnabled(false);

  ITexture2DArray *txt = vkQueryClass<ITexture2DArray>(m_lightRenderers[eLT_DirectionalLight]->GetShadowBuffer()->GetColorBuffer(0));
  //m_renderer->RenderFullScreenFrame(0.0f, 0.25, 0.0f, 0.25f, txt, 0);
  //m_renderer->RenderFullScreenFrame(0.25f, 0.5, 0.0, 0.25, txt, 1);
  //m_renderer->RenderFullScreenFrame(0.5f, 0.75, 0.0f, 0.25f, txt, 2);
  //m_renderer->RenderFullScreenFrame(txt, 1);
  
  m_renderer->SetBlendEnabled(false);

}

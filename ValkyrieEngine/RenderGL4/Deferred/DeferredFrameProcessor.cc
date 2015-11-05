
#include <RenderGL4/Deferred/DeferredFrameProcessor.hh>
#include <RenderGL4/RendererGL4.hh>
#include <RenderGL4/TextureGL4.hh>
#include <RenderGL4/Deferred/GBuffer.hh>
#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/Scene/Node.hh>
#include <Valkyrie/Graphics/Scene/GeometryNode.hh>
#include <GL/glew.h>


vkDeferredFrameProcessor::vkDeferredFrameProcessor(RendererGL4 *renderer)
  : IFrameProcessor()
  , m_renderer(renderer)
  , m_geometries(64, 16)
  , m_gbuffer(0)
{
  VK_CLASS_GEN_CONSTR;


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


class Collector : public IScanCallback
{
public:
  Collector(vkCollection<vkGeometryNode*> &geometrieNodes)
    : m_geometryNodes(geometrieNodes)
  {

  }

  virtual bool ScanGeometryNode(vkGeometryNode *geometryNode)
  {
    m_geometryNodes.Add(geometryNode);
    return true;
  }

  virtual bool ScanLightNode(vkLightNode *lightNode)
  {
    return true;
  }


private:
  vkCollection<vkGeometryNode*> &m_geometryNodes;
};


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

void vkDeferredFrameProcessor::Render(vkNode *node, IRenderTarget *target)
{
  m_geometries.Clear();
  Collector collector(m_geometries);
  node->Scan(0, m_renderer, &collector);


  RenderGBuffer(node);


  // now simply present the scene
  RenderDirectionalLight(target);

}

void vkDeferredFrameProcessor::RenderSimplePresent(IRenderTarget *target)
{
  m_renderer->SetRenderTarget(target);
  GLenum buffers[] = {
    GL_COLOR_ATTACHMENT0, // Diffuse / Roughness
  };
  glDrawBuffers(1, buffers);

  static vkShaderAttributeID diffuseRoughnessAttr("DiffuseRoughness");
  static vkShaderAttributeID normalLightModeAttr("NormalLightMode");
  static vkShaderAttributeID emissiveMetallicAttr("EmissiveMetallic");
  static vkShaderAttributeID sssSpecularAttr("SSSSpecular");
  static vkShaderAttributeID depthAttr("Depth");

  m_renderer->SetShader(m_simplePresentShader);

  IShaderAttribute *attrDiffuseRoughness = m_simplePresentShader->GetAttribute(diffuseRoughnessAttr);
  IShaderAttribute *attrNormalLightMode = m_simplePresentShader->GetAttribute(normalLightModeAttr);
  IShaderAttribute *attrEmissiveMetallic = m_simplePresentShader->GetAttribute(emissiveMetallicAttr);
  IShaderAttribute *attrSSSSpecular = m_simplePresentShader->GetAttribute(sssSpecularAttr);
  IShaderAttribute *attrDepth = m_simplePresentShader->GetAttribute(depthAttr);

  if (attrDiffuseRoughness)
  {
    vkTextureUnit tuDiffuseRoughness = m_renderer->BindTexture(m_gbuffer->GetDiffuseRoughness());
    attrDiffuseRoughness->Set(tuDiffuseRoughness);
  }
  if (attrNormalLightMode)
  {
    vkTextureUnit tuNormalLightMode = m_renderer->BindTexture(m_gbuffer->GetNormalLightMode());
    attrNormalLightMode->Set(tuNormalLightMode);
  }
  if (attrEmissiveMetallic)
  {
    vkTextureUnit tuEmissiveMetallic = m_renderer->BindTexture(m_gbuffer->GetEmissiveMetallic());
    attrEmissiveMetallic->Set(tuEmissiveMetallic);
  }
  if (attrSSSSpecular)
  {
    vkTextureUnit tuSSSSpecular = m_renderer->BindTexture(m_gbuffer->GetSSSSpec());
    attrSSSSpecular->Set(tuSSSSpecular);
  }
  if (attrDepth)
  {
    vkTextureUnit tuDepth = m_renderer->BindTexture(m_gbuffer->GetDepth());
    attrDepth->Set(tuDepth);
  }
  m_renderer->RenderFullScreenFrame();

}


void vkDeferredFrameProcessor::RenderDirectionalLight(IRenderTarget *target)
{
  m_renderer->SetRenderTarget(target);
  m_renderer->Clear();
  GLenum buffers[] = {
    GL_COLOR_ATTACHMENT0, // Diffuse / Roughness
  };
  glDrawBuffers(1, buffers);

  static vkShaderAttributeID diffuseRoughnessAttr("DiffuseRoughness");
  static vkShaderAttributeID normalLightModeAttr("NormalLightMode");
  static vkShaderAttributeID emissiveMetallicAttr("EmissiveMetallic");
  static vkShaderAttributeID sssSpecularAttr("SSSSpecular");
  static vkShaderAttributeID depthAttr("Depth");

  m_renderer->SetShader(m_directionLightShader);

  IShaderAttribute *attrDiffuseRoughness = m_directionLightShader->GetAttribute(diffuseRoughnessAttr);
  IShaderAttribute *attrNormalLightMode = m_directionLightShader->GetAttribute(normalLightModeAttr);
  IShaderAttribute *attrEmissiveMetallic = m_directionLightShader->GetAttribute(emissiveMetallicAttr);
  IShaderAttribute *attrSSSSpecular = m_directionLightShader->GetAttribute(sssSpecularAttr);
  IShaderAttribute *attrDepth = m_directionLightShader->GetAttribute(depthAttr);

  if (attrDiffuseRoughness)
  {
    vkTextureUnit tuDiffuseRoughness = m_renderer->BindTexture(m_gbuffer->GetDiffuseRoughness());
    attrDiffuseRoughness->Set(tuDiffuseRoughness);
  }
  if (attrNormalLightMode)
  {
    vkTextureUnit tuNormalLightMode = m_renderer->BindTexture(m_gbuffer->GetNormalLightMode());
    attrNormalLightMode->Set(tuNormalLightMode);
  }
  if (attrEmissiveMetallic)
  {
    vkTextureUnit tuEmissiveMetallic = m_renderer->BindTexture(m_gbuffer->GetEmissiveMetallic());
    attrEmissiveMetallic->Set(tuEmissiveMetallic);
  }
  if (attrSSSSpecular)
  {
    vkTextureUnit tuSSSSpecular = m_renderer->BindTexture(m_gbuffer->GetSSSSpec());
    attrSSSSpecular->Set(tuSSSSpecular);
  }
  if (attrDepth)
  {
    vkTextureUnit tuDepth = m_renderer->BindTexture(m_gbuffer->GetDepth());
    attrDepth->Set(tuDepth);
  }
  m_renderer->RenderFullScreenFrame();

}



#include <RenderGL4/Deferred/DeferredFrameProcessor.hh>
#include <RenderGL4/RendererGL4.hh>
#include <RenderGL4/Deferred/GBuffer.hh>
#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <Valkyrie/Graphics/Scene/Node.hh>
#include <Valkyrie/Graphics/Scene/GeometryNode.hh>



vkDeferredFrameProcessor::vkDeferredFrameProcessor(RendererGL4 *renderer)
  : IFrameProcessor()
  , m_renderer(renderer)
  , m_geometries(64, 16)
{
  VK_CLASS_GEN_CONSTR;

  vkGBuffer *gbuffer = new vkGBuffer(renderer, 1366, 768);
}

vkDeferredFrameProcessor::~vkDeferredFrameProcessor()
{

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

void vkDeferredFrameProcessor::Render(vkNode *node, IRenderTarget *target)
{
  m_geometries.Clear();
  Collector collector(m_geometries);
  node->Scan(0, m_renderer, &collector);

  printf("Number of nodes: %llu\n", m_geometries.length);
}

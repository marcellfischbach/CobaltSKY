
#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Graphics/IFrameProcessor.hh>
#include <RenderGL4/Deferred/DeferredFrameProcessor.refl.hh>

struct IShader;
class vkGBuffer;
class vkGeometryNode;
class vkLightNode;
class RendererGL4;
class vkLightRendererGL4;

VK_INTERFACE();
class VKGL4_API vkDeferredFrameProcessor : public virtual IFrameProcessor
{
  VK_CLASS_GEN_OBJECT;
public:
  vkDeferredFrameProcessor(RendererGL4 *renderer);
  virtual ~vkDeferredFrameProcessor();

  bool Initialize(vkUInt16 width, vkUInt16 height);

  void Render(vkNode *node, const vkCamera *camera, IRenderTarget *target);

private:
  void RenderGBuffer(vkNode *rootNode);

  void RenderDirectionalLight(IRenderTarget *target);
  void RenderSimplePresent(IRenderTarget *target);

private:
  vkCollection<vkGeometryNode*> m_geometries;
  vkCollection<vkLightNode*> m_lights;
  RendererGL4 *m_renderer;

  vkGBuffer *m_gbuffer;

  IShader *m_simplePresentShader;
  IShader *m_directionLightShader;

  vkLightRendererGL4 *m_lightRenderers[eLT_Count];
};

#pragma once

#include <GraphicsGL4/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Graphics/IFrameProcessor.hh>
#include <GraphicsGL4/Deferred/DefaultCollector.hh>
#include <GraphicsGL4/Deferred/DeferredFrameProcessor.refl.hh>

struct IShader;
class vkGBuffer;
class vkGraphicsGL4;
class vkLight;
class vkLightvkGraphicsGL4;
class vkGeometryMesh;

VK_INTERFACE();
class VKGL4_API vkDeferredFrameProcessor : public virtual IFrameProcessor
{
  VK_CLASS_GEN_OBJECT;
public:
  vkDeferredFrameProcessor(vkGraphicsGL4 *renderer);
  virtual ~vkDeferredFrameProcessor();

  bool Initialize(vkUInt16 width, vkUInt16 height);

  void Render(vkEntity *root, vkCamera *camera, IRenderTarget *target);

private:
  void RenderGBuffer(vkEntity *root);

private:
  vkCollection<vkGeometryMesh*> m_geometries;
  vkCollection<vkLight*> m_lights;
  vkGraphicsGL4 *m_renderer;

  vkGBuffer *m_gbuffer;

  IShader *m_simplePresentShader;
  IShader *m_directionLightShader;

  vkLightvkGraphicsGL4 *m_lightRenderers[eLT_Count];
};
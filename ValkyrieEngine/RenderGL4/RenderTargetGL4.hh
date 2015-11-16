#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <vector>
#include <GL/glew.h>
#include <RenderGL4/RenderTargetGL4.refl.hh>

class vkTextureGL4;

VK_CLASS();
class VKGL4_API vkRenderTargetGL4 : public IRenderTarget
{
  VK_CLASS_GEN_OBJECT;

public:
  vkRenderTargetGL4();
  virtual ~vkRenderTargetGL4();

  void Bind();


  void AddColorTexture(ITexture *color);
  void SetDepthTexture(ITexture *depth);
  void SetDepthBuffer(vkUInt16 width, vkUInt16 height);
  void Initialize();
  bool Finilize();


  virtual ITexture *GetColorBuffer(vkUInt8 buffer) const;
  virtual ITexture *GetDepthBuffer() const;

private:

  std::vector<vkTextureGL4*> m_colorTextures;
  vkTextureGL4 *m_depthTexture;
  GLuint m_depthBuffer;
  GLuint m_name;
};



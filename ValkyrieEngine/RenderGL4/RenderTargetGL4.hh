#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <vector>
#include <GL/glew.h>
#include <RenderGL4/RenderTargetGL4.refl.hh>

class vkTexture2DGL4;

VK_CLASS();
class VKGL4_API vkRenderTargetGL4 : public IRenderTarget
{
  VK_CLASS_GEN_OBJECT;

public:
  vkRenderTargetGL4();
  virtual ~vkRenderTargetGL4();

  void Bind();


  void AddColorTexture(ITexture2D *color);
  void SetDepthTexture(ITexture2D *depth);
  void SetDepthBuffer(vkUInt16 width, vkUInt16 height);
  void Initialize();
  bool Finilize();


  virtual ITexture2D *GetColorBuffer(vkUInt8 buffer) const;
  virtual ITexture2D *GetDepthBuffer() const;

private:

  std::vector<vkTexture2DGL4*> m_colorTextures;
  vkTexture2DGL4 *m_depthTexture;
  GLuint m_depthBuffer;
  GLuint m_name;
};



#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Graphics/IFrameBuffer.hh>
#include <vector>
#include <GL/glew.h>

class vkTexture2DGL4;

VK_CLASS();
class VKGL4_API vkFrameBufferGL4 : public IFrameBuffer
{
  VK_CLASS_GEN_OBJECT;

public:
  vkFrameBufferGL4();
  virtual ~vkFrameBufferGL4();

  void AddColorTexture(vkTexture2DGL4 *color);
  void SetDepthTexture(vkTexture2DGL4 *depth);
  void SetDepthBuffer(vkUInt16 width, vkUInt16 height);
  void Initialize();
  bool Finilize();


  virtual ITexture2D *GetColorBuffer(vkUInt8 buffer) const;
  virtual ITexture2D *GetDepthBuffer(vkUInt8 buffer) const;

private:
  std::vector<vkTexture2DGL4*> m_colorBuffers;
  vkTexture2DGL4 *m_depthBuffer;

  GLuint m_name;
};


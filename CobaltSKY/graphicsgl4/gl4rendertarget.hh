#pragma once

#include <graphicsgl4/gl4export.hh>
#include <valkyrie/graphics/irendertarget.hh>
#include <vector>
#include <GL/glew.h>
#include <graphicsgl4/gl4rendertarget.refl.hh>

class csTextureGL4;

CS_CLASS()
class CSGRAPHICSGL4_API csRenderTargetGL4 : public CS_SUPER(iRenderTarget)
{
  CS_CLASS_GEN_OBJECT;

public:
  csRenderTargetGL4();
  csRenderTargetGL4(GLuint name, csUInt16 width, csUInt16 height);
  virtual ~csRenderTargetGL4();

  void Setup(GLuint name, csUInt16 width, csUInt16 heigth);
  void Bind();


  csUInt16 GetWidth() const;
  csUInt16 GetHeight() const;


  void AddColorTexture(iTexture *color);
  void SetDepthTexture(iTexture *depth);
  void SetDepthBuffer(csUInt16 width, csUInt16 height);
  void Initialize(csUInt16 width, csUInt16 height);
  bool Finilize();


  virtual iTexture *GetColorBuffer(csUInt8 buffer) const;
  virtual iTexture *GetDepthBuffer() const;

private:
  csUInt16 m_width;
  csUInt16 m_height;

  std::vector<csTextureGL4*> m_colorTextures;
  csTextureGL4 *m_depthTexture;
  GLuint m_depthBuffer;
  GLuint m_name;
  bool m_provided;
};



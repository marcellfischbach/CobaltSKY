#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/graphics/irendertarget.hh>
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


  void AddColorTexture(csTextureWrapper *color);
  void SetDepthTexture(csTextureWrapper *depth);
  void SetDepthBuffer(csUInt16 width, csUInt16 height);
  void Initialize(csUInt16 width, csUInt16 height);
  bool Finilize();


  virtual csTextureWrapper *GetColorBuffer(csUInt8 buffer) const;
  virtual csTextureWrapper *GetDepthBuffer() const;

private:
  csUInt16 m_width;
  csUInt16 m_height;

  std::vector<csTextureWrapper*> m_colorTextures;
  csTextureWrapper *m_depthTexture;
  GLuint m_depthBuffer;
  GLuint m_name;
  bool m_provided;
};



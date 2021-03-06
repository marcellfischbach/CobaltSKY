#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/graphics/irendertarget.hh>
#include <vector>
#include <GL/glew.h>
#include <graphicsgl4/gl4rendertarget.refl.hh>


namespace cs
{
class TextureGL4;

CS_CLASS()
class CSGRAPHICSGL4_API RenderTargetGL4 : public CS_SUPER(cs::iRenderTarget)
{
  CS_CLASS_GEN_OBJECT;

public:
  RenderTargetGL4();
  RenderTargetGL4(GLuint name, csUInt16 width, csUInt16 height);
  virtual ~RenderTargetGL4();

  void Setup(GLuint name, csUInt16 width, csUInt16 heigth);
  void Bind();


  csUInt16 GetWidth() const;
  csUInt16 GetHeight() const;


  void AddColorTexture(cs::TextureWrapper * color);
  void SetDepthTexture(cs::TextureWrapper * depth);
  void SetDepthBuffer(csUInt16 width, csUInt16 height);
  void Initialize(csUInt16 width, csUInt16 height);
  bool Finilize();


  virtual cs::TextureWrapper* GetColorBuffer(csUInt8 buffer) const;
  virtual cs::TextureWrapper* GetDepthBuffer() const;

private:
  csUInt16 m_width;
  csUInt16 m_height;

  std::vector<cs::TextureWrapper*> m_colorTextures;
  cs::TextureWrapper* m_depthTexture;
  GLuint m_depthBuffer;
  GLuint m_name;
  bool m_provided;
};



}
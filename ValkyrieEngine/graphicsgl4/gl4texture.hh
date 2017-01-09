#pragma once


#include <graphicsgl4/gl4export.hh>
#include <valkyrie/graphics/itexture.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4texture.refl.hh>

VK_INTERFACE()
class VKGRAPHICSGL4_API vkTextureGL4 : public virtual VK_SUPER(iTexture)
{
  VK_CLASS_GEN_OBJECT;
public:
  virtual ~vkTextureGL4();

  virtual bool Initialize();

  virtual vkTextureType GetType() const ;

  virtual void SetSampler(iSampler *sampler);
  virtual iSampler *GetSampler();
  virtual const iSampler *GetSampler() const;

  virtual void GenerateMipMaps();

  void Bind();
  GLuint GetName() const;

protected:
  vkTextureGL4(vkTextureType type);

  GLuint m_name;
  GLenum m_target;

private:
  vkTextureType m_type;

  iSampler *m_sampler;

};



VK_FORCEINLINE GLuint vkTextureGL4::GetName() const
{
  return m_name;
}


#pragma once


#include <GraphicsGL4/gl4export.hh>
#include <valkyrie/graphics/itexture.hh>
#include <GL/glew.h>
#include <GraphicsGL4/gl4texture.refl.hh>

VK_INTERFACE()
class VKGRAPHICSGL4_API vkTextureGL4 : public virtual VK_SUPER(ITexture)
{
  VK_CLASS_GEN_OBJECT;
public:
  virtual ~vkTextureGL4();

  virtual bool Initialize();

  virtual vkTextureType GetType() const ;

  virtual void SetSampler(ISampler *sampler);
  virtual ISampler *GetSampler();
  virtual const ISampler *GetSampler() const;

  virtual void GenerateMipMaps();

  void Bind();
  GLuint GetName() const;

protected:
  vkTextureGL4(vkTextureType type);

  GLuint m_name;
  GLenum m_target;

private:
  vkTextureType m_type;

  ISampler *m_sampler;

};



VK_FORCEINLINE GLuint vkTextureGL4::GetName() const
{
  return m_name;
}


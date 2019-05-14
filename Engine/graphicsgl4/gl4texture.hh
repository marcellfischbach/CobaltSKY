#pragma once


#include <graphicsgl4/gl4export.hh>
#include <cobalt/graphics/itexture.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4texture.refl.hh>

CS_CLASS()
class CSGRAPHICSGL4_API csTextureGL4 : public virtual CS_SUPER(iTexture)
{
  CS_CLASS_GEN_OBJECT;
public:
  virtual ~csTextureGL4();

  virtual bool Initialize();

  virtual csTextureType GetType() const ;

  virtual void SetSampler(csSamplerWrapper *sampler);
  virtual csSamplerWrapper *GetSampler();
  virtual const csSamplerWrapper *GetSampler() const;

  virtual void GenerateMipMaps();

  void Bind();
  GLuint GetName() const;

protected:
  csTextureGL4(csTextureType type);

  GLuint m_name;
  GLenum m_target;

private:
  csTextureType m_type;

  csSamplerWrapper *m_sampler;

};



CS_FORCEINLINE GLuint csTextureGL4::GetName() const
{
  return m_name;
}


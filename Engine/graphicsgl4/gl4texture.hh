#pragma once


#include <graphicsgl4/gl4export.hh>
#include <cobalt/graphics/itexture.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4texture.refl.hh>

CS_CLASS()
class CSGRAPHICSGL4_API csTextureGL4 : public virtual CS_SUPER(cs::iTexture)
{
  CS_CLASS_GEN_OBJECT;
public:
  virtual ~csTextureGL4();

  virtual bool Initialize();

  virtual cs::eTextureType GetType() const ;

  virtual void SetSampler(cs::SamplerWrapper *sampler);
  virtual cs::SamplerWrapper *GetSampler();
  virtual const cs::SamplerWrapper *GetSampler() const;

  virtual void GenerateMipMaps();

  void Bind();
  GLuint GetName() const;

protected:
  csTextureGL4(cs::eTextureType type);

  GLuint m_name;
  GLenum m_target;

private:
  cs::eTextureType m_type;

  cs::SamplerWrapper *m_sampler;

};



CS_FORCEINLINE GLuint csTextureGL4::GetName() const
{
  return m_name;
}


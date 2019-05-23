#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4texture.hh>
#include <cobalt/graphics/isampler.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4sampler.refl.hh>


namespace cs
{

CS_CLASS();
class CSGRAPHICSGL4_API SamplerGL4 : public CS_SUPER(cs::iSampler)
{
  CS_CLASS_GEN_OBJECT;

public:
  SamplerGL4();
  virtual ~SamplerGL4();

  bool Initialize();

  void Bind(GLuint unit);

  virtual void SetFilter(cs::eFilterMode filter);
  virtual cs::eFilterMode GetFilter() const;

  virtual void SetAnisotropy(csUInt8 anisotropy);
  virtual csUInt8 GetAnisotropy() const;

  virtual void SetMinLOD(csInt16 minLOD);
  virtual csInt16 GetMinLOD() const;

  virtual void SetMaxLOD(csInt16 minLOD);
  virtual csInt16 GetMaxLOD() const;

  virtual void SetAddressU(cs::eTextureAddressMode addressMode);
  virtual cs::eTextureAddressMode GetAddressU() const;

  virtual void SetAddressV(cs::eTextureAddressMode addressMode);
  virtual cs::eTextureAddressMode GetAddressV() const;

  virtual void SetAddressW(cs::eTextureAddressMode addressMode);
  virtual cs::eTextureAddressMode GetAddressW() const;

  virtual void SetBorderColor(const cs::Vector4f & color);
  virtual const cs::Vector4f& GetBorderColor() const;

  virtual void SetTextureCompareMode(cs::eTextureCompareMode mode);
  virtual cs::eTextureCompareMode GetTextureCompareMode() const;

  virtual void SetTextureCompareFunc(cs::eTextureCompareFunc func);
  virtual cs::eTextureCompareFunc GetTextureCompareFunc() const;

  virtual bool NeedsMipMaps() const;
private:
  GLuint m_name;

  cs::eFilterMode m_filterMode;
  csUInt8 m_anisotropy;
  csInt16 m_minLOD;
  csInt16 m_maxLOD;
  cs::eTextureAddressMode m_addressU;
  cs::eTextureAddressMode m_addressV;
  cs::eTextureAddressMode m_addressW;
  cs::Vector4f m_borderColor;
  cs::eTextureCompareMode m_textureCompareMode;
  cs::eTextureCompareFunc m_textureCompareFunc;
};

}
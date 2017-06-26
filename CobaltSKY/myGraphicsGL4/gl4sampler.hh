#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4texture.hh>
#include <cobalt/graphics/isampler.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4sampler.refl.hh>

CS_CLASS();
class CSGRAPHICSGL4_API csSamplerGL4 : public CS_SUPER(iSampler)
{
  CS_CLASS_GEN_OBJECT;

public:
  csSamplerGL4();
  virtual ~csSamplerGL4();

  bool Initialize();

  void Bind(GLuint unit);

  virtual void SetFilter(csFilterMode filter);
  virtual csFilterMode GetFilter() const;

  virtual void SetAnisotropy(csUInt8 anisotropy);
  virtual csUInt8 GetAnisotropy() const;

  virtual void SetMinLOD(csInt16 minLOD);
  virtual csInt16 GetMinLOD() const;

  virtual void SetMaxLOD(csInt16 minLOD);
  virtual csInt16 GetMaxLOD() const;

  virtual void SetAddressU(csTextureAddressMode addressMode);
  virtual csTextureAddressMode GetAddressU() const;

  virtual void SetAddressV(csTextureAddressMode addressMode);
  virtual csTextureAddressMode GetAddressV() const;

  virtual void SetAddressW(csTextureAddressMode addressMode);
  virtual csTextureAddressMode GetAddressW() const;

  virtual void SetBorderColor(const csVector4f &color);
  virtual const csVector4f &GetBorderColor() const;

  virtual void SetTextureCompareMode(csTextureCompareMode mode);
  virtual csTextureCompareMode GetTextureCompareMode() const;

  virtual void SetTextureCompareFunc(csTextureCompareFunc func);
  virtual csTextureCompareFunc GetTextureCompareFunc() const;

  virtual bool NeedsMipMaps() const;
private:
  GLuint m_name;

  csFilterMode m_filterMode;
  csUInt8 m_anisotropy;
  csInt16 m_minLOD;
  csInt16 m_maxLOD;
  csTextureAddressMode m_addressU;
  csTextureAddressMode m_addressV;
  csTextureAddressMode m_addressW;
  csVector4f m_borderColor;
  csTextureCompareMode m_textureCompareMode;
  csTextureCompareFunc m_textureCompareFunc;
};

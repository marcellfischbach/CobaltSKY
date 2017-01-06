#pragma once


#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/gl4texture.hh>
#include <valkyrie/graphics/isampler.hh>
#include <GL/glew.h>
#include <GraphicsGL4/gl4sampler.refl.hh>

VK_CLASS();
class VKGRAPHICSGL4_API vkSamplerGL4 : public VK_SUPER(ISampler)
{
  VK_CLASS_GEN_OBJECT;

public:
  vkSamplerGL4();
  virtual ~vkSamplerGL4();

  bool Initialize();

  void Bind(GLuint unit);

  virtual void SetFilter(vkFilterMode filter);
  virtual vkFilterMode GetFilter() const;

  virtual void SetAnisotropy(vkUInt8 anisotropy);
  virtual vkUInt8 GetAnisotropy() const;

  virtual void SetMinLOD(vkInt16 minLOD);
  virtual vkInt16 GetMinLOD() const;

  virtual void SetMaxLOD(vkInt16 minLOD);
  virtual vkInt16 GetMaxLOD() const;

  virtual void SetAddressU(vkTextureAddressMode addressMode);
  virtual vkTextureAddressMode GetAddressU() const;

  virtual void SetAddressV(vkTextureAddressMode addressMode);
  virtual vkTextureAddressMode GetAddressV() const;

  virtual void SetAddressW(vkTextureAddressMode addressMode);
  virtual vkTextureAddressMode GetAddressW() const;

  virtual void SetBorderColor(const vkVector4f &color);
  virtual const vkVector4f &GetBorderColor() const;

  virtual void SetTextureCompareMode(vkTextureCompareMode mode);
  virtual vkTextureCompareMode GetTextureCompareMode() const;

  virtual void SetTextureCompareFunc(vkTextureCompareFunc func);
  virtual vkTextureCompareFunc GetTextureCompareFunc() const;

  virtual bool NeedsMipMaps() const;
private:
  GLuint m_name;

  vkFilterMode m_filterMode;
  vkUInt8 m_anisotropy;
  vkInt16 m_minLOD;
  vkInt16 m_maxLOD;
  vkTextureAddressMode m_addressU;
  vkTextureAddressMode m_addressV;
  vkTextureAddressMode m_addressW;
  vkVector4f m_borderColor;
  vkTextureCompareMode m_textureCompareMode;
  vkTextureCompareFunc m_textureCompareFunc;
};

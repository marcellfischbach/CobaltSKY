#pragma once


#include <RenderGL4/Export.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <GL/glew.h>
#include <RenderGL4/TextureGL4.refl.hh>

VK_CLASS();
class VKGL4_API vkSamplerGL4 : public ISampler
{
  VK_CLASS_GEN_OBJECT;

public:
  vkSamplerGL4();
  virtual ~vkSamplerGL4();

  bool Initialize();

  virtual void SetFilter(vkFilterMode filter);
  virtual vkFilterMode GetFilter() const;

  virtual void SetAnisotropy(vkUInt8 anisotropy);
  virtual vkUInt8 GetAnisotropy() const;

  virtual void SetMinLOD(vkUInt16 minLOD);
  virtual vkUInt16 GetMinLOD() const;

  virtual void SetMaxLOD(vkUInt16 minLOD);
  virtual vkUInt16 GetMaxLOD() const;

  virtual void SetAddressU(vkTextureAddressMode addressMode);
  virtual vkTextureAddressMode GetAddressU() const;

  virtual void SetAddressV(vkTextureAddressMode addressMode);
  virtual vkTextureAddressMode GetAddressV() const;

  virtual void SetAddressW(vkTextureAddressMode addressMode);
  virtual vkTextureAddressMode GetAddressW() const;

  virtual void SetBorderColor(const vkVector4f &color);
  virtual const vkVector4f &GetBorderColor() const;

private:
  GLuint m_name;

  vkFilterMode m_filterMode;
  vkUInt8 m_anisotropy;
  vkUInt16 m_minLOD;
  vkUInt16 m_maxLOD;
  vkTextureAddressMode m_addressU;
  vkTextureAddressMode m_addressV;
  vkTextureAddressMode m_addressW;
  vkVector4f m_borderColor;
};

VK_INTERFACE();
class VKGL4_API vkTextureGL4 : public virtual IObject
{
public:
  vkTextureGL4(GLenum target);
  virtual ~vkTextureGL4();

  void Bind();

protected:
  GLuint m_name;

private:
  GLenum m_target;

};

VK_CLASS();
class VKGL4_API vkTexture2DGL4 : public ITexture2D
{
  VK_CLASS_GEN_OBJECT;
public:

  vkTexture2DGL4();
  virtual ~vkTexture2DGL4();

  void Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height);

  // Implementation of the ITexture interface
  vkTextureType GetType() const;

  virtual void SetSampler(ISampler *sampler);
  virtual ISampler* GetSampler();
  virtual const ISampler* GetSampler() const;

  // Implementation of the ITexture2D interface
  virtual vkUInt16 GetWidth() const;
  virtual vkUInt16 GetHeight() const;

  virtual void CopyData(vkUInt8 layer, vkSize size, const void *data);


private:
  ISampler *m_sampler;

};


vkTextureType vkTexture2DGL4::GetType() const
{
  return eTT_Texture2D;
}


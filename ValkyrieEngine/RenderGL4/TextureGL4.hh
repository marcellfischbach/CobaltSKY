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

  void Bind(GLuint unit);

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

  virtual void SetTextureCompareMode(vkTextureCompareMode mode);
  virtual vkTextureCompareMode GetTextureCompareMode() const;

  virtual void SetTextureCompareFunc(vkTextureCompareFunc func);
  virtual vkTextureCompareFunc GetTextureCompareFunc() const;

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
  vkTextureCompareMode m_textureCompareMode;
  vkTextureCompareFunc m_textureCompareFunc;
};

VK_INTERFACE();
class VKGL4_API vkTextureGL4 : public virtual ITexture
{
  VK_CLASS_GEN_OBJECT;
public:
  virtual ~vkTextureGL4();

  virtual bool Initialize();

  virtual vkTextureType GetType() const ;

  virtual void SetSampler(ISampler *sampler);
  virtual ISampler *GetSampler();
  virtual const ISampler *GetSampler() const;


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

VK_CLASS();
class VKGL4_API vkTexture2DGL4 : public virtual vkTextureGL4, public virtual ITexture2D
{
  VK_CLASS_GEN;
public:

  vkTexture2DGL4();
  virtual ~vkTexture2DGL4();

  bool Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height);


  // Implementation of the ITexture2D interface
  virtual vkUInt16 GetWidth() const;
  virtual vkUInt16 GetHeight() const;

  virtual bool CopyData(vkUInt8 layer, vkPixelFormat format, const void *data);

private:
  vkPixelFormat m_format;

  vkUInt16 m_width;
  vkUInt16 m_height;
};

VK_CLASS();
class VKGL4_API vkTexture2DArrayGL4 : public virtual vkTextureGL4, public virtual ITexture2DArray
{
  VK_CLASS_GEN;
public:

  vkTexture2DArrayGL4();
  virtual ~vkTexture2DArrayGL4();

  bool Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 layers);


  // Implementation of the ITexture2DArray interface
  virtual vkUInt16 GetWidth() const;
  virtual vkUInt16 GetHeight() const;
  virtual vkUInt16 GetLayers() const;

  virtual bool CopyData(vkUInt16 layer, vkUInt8 lod, vkPixelFormat format, const void *data);

private:
  vkPixelFormat m_format;

  vkUInt16 m_width;
  vkUInt16 m_height;
  vkUInt16 m_layers;
};


VK_CLASS();
class VKGL4_API vkTextureCubeGL4 : public virtual vkTextureGL4, public virtual ITextureCube
{
  VK_CLASS_GEN;
public:

  vkTextureCubeGL4();
  virtual ~vkTextureCubeGL4();

  bool Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 depth);


  // Implementation of the ITexture2DArray interface
  virtual vkUInt16 GetWidth() const;
  virtual vkUInt16 GetHeight() const;
  virtual vkUInt16 GetDepth() const;

  virtual bool CopyData(vkTextureCubeFace face, vkUInt8 lod, vkPixelFormat format, const void *data);

private:
  vkPixelFormat m_format;

  vkUInt16 m_width;
  vkUInt16 m_height;
  vkUInt16 m_depth;
};




VK_FORCEINLINE GLuint vkTextureGL4::GetName() const
{
  return m_name;
}


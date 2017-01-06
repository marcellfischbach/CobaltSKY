#pragma once


#include <GraphicsGL4/Export.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/graphics/itexture2darray.hh>
#include <valkyrie/graphics/itexturecube.hh>
#include <valkyrie/graphics/isampler.hh>
#include <GL/glew.h>
#include <GraphicsGL4/TextureGL4.refl.hh>

VK_CLASS();
class VKGL4_API vkSamplerGL4 : public VK_SUPER(ISampler)
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

VK_INTERFACE()
class VKGL4_API vkTextureGL4 : public virtual VK_SUPER(ITexture)
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

VK_CLASS()
class VKGL4_API vkTexture2DGL4 : public VK_SUPER(vkTextureGL4), public VK_SUPER(ITexture2D)
{
  VK_CLASS_GEN;
public:

  vkTexture2DGL4();
  virtual ~vkTexture2DGL4();

  bool Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height, bool mipmaps);


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
class VKGL4_API vkTexture2DArrayGL4 : public VK_SUPER(vkTextureGL4), public VK_SUPER(ITexture2DArray)
{
  VK_CLASS_GEN;
public:

  vkTexture2DArrayGL4();
  virtual ~vkTexture2DArrayGL4();

  bool Initialize(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 layers, bool mipmaps);


  // Implementation of the ITexture2DArray interface
  virtual vkUInt16 GetWidth() const;
  virtual vkUInt16 GetHeight() const;
  virtual vkUInt16 GetLayers() const;

  virtual bool CopyData(vkUInt8 lod, vkPixelFormat format, const void *data);
  virtual bool CopyData(vkUInt16 layer, vkUInt8 lod, vkPixelFormat format, const void *data);

private:
  vkPixelFormat m_format;

  vkUInt16 m_width;
  vkUInt16 m_height;
  vkUInt16 m_layers;
};


VK_CLASS();
class VKGL4_API vkTextureCubeGL4 : public VK_SUPER(vkTextureGL4), public VK_SUPER(ITextureCube)
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


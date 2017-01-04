#pragma once

#pragma warning(disable: 4250)


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/Core/vkclass.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Graphics/ITexture.refl.hh>

VK_INTERFACE();
struct VKE_API ISampler : public IObject
{
  VK_CLASS_GEN;


  virtual void SetFilter(vkFilterMode filter) = 0;
  virtual vkFilterMode GetFilter() const = 0;

  virtual void SetAnisotropy(vkUInt8 anisotropy) = 0;
  virtual vkUInt8 GetAnisotropy() const = 0;

  virtual void SetMinLOD(vkInt16 minLOD) = 0;
  virtual vkInt16 GetMinLOD() const = 0;

  virtual void SetMaxLOD(vkInt16 minLOD) = 0;
  virtual vkInt16 GetMaxLOD() const = 0;

  virtual void SetAddressU(vkTextureAddressMode addressMode) = 0;
  virtual vkTextureAddressMode GetAddressU() const = 0;

  virtual void SetAddressV(vkTextureAddressMode addressMode) = 0;
  virtual vkTextureAddressMode GetAddressV() const = 0;

  virtual void SetAddressW(vkTextureAddressMode addressMode) = 0;
  virtual vkTextureAddressMode GetAddressW() const = 0;

  virtual void SetBorderColor(const vkVector4f &color) = 0;
  virtual const vkVector4f &GetBorderColor() const = 0;

  virtual void SetTextureCompareMode(vkTextureCompareMode mode) = 0;
  virtual vkTextureCompareMode GetTextureCompareMode() const = 0;

  virtual void SetTextureCompareFunc(vkTextureCompareFunc func) = 0;
  virtual vkTextureCompareFunc GetTextureCompareFunc() const = 0;

  virtual bool NeedsMipMaps() const = 0;

};

VK_INTERFACE();
struct VKE_API ITexture : public IObject
{
  VK_CLASS_GEN;
  ITexture() : IObject() { }
  virtual ~ITexture() { }

  virtual vkTextureType GetType() const = 0;

  virtual void SetSampler(ISampler *sampler) = 0;
  virtual ISampler* GetSampler() = 0;
  virtual const ISampler* GetSampler() const = 0;

  virtual void GenerateMipMaps() = 0;

};

VK_INTERFACE();
struct VKE_API ITexture2D : public virtual VK_SUPER(ITexture)
{
  VK_CLASS_GEN;
  ITexture2D() : ITexture() { }

  virtual ~ITexture2D() { }

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;

  virtual bool CopyData(vkUInt8 lod, vkPixelFormat format, const void *data) = 0;

};

VK_INTERFACE();
struct VKE_API ITexture2DArray : public virtual VK_SUPER(ITexture)
{
  VK_CLASS_GEN;
  ITexture2DArray() : ITexture() { }

  virtual ~ITexture2DArray() { }

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;
  virtual vkUInt16 GetLayers() const = 0;


  virtual bool CopyData(vkUInt8 lod, vkPixelFormat format, const void *data) = 0;
  virtual bool CopyData(vkUInt16 layer, vkUInt8 lod, vkPixelFormat format, const void *data) = 0;

};



VK_INTERFACE();
struct VKE_API ITextureCube : public virtual VK_SUPER(ITexture)
{
  VK_CLASS_GEN;
  ITextureCube() : ITexture() { }

  virtual ~ITextureCube() { }

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;
  virtual vkUInt16 GetDepth() const = 0;

  virtual bool CopyData(vkTextureCubeFace face, vkUInt8 lod, vkPixelFormat format, const void *data) = 0;

};
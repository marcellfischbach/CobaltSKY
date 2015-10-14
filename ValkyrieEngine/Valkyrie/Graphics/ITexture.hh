#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/Object.hh>
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

  virtual void SetMinLOD(vkUInt16 minLOD) = 0;
  virtual vkUInt16 GetMinLOD() const = 0;

  virtual void SetMaxLOD(vkUInt16 minLOD) = 0;
  virtual vkUInt16 GetMaxLOD() const = 0;

  virtual void SetAddressU(vkTextureAddressMode addressMode) = 0;
  virtual vkTextureAddressMode GetAddressU() const = 0;

  virtual void SetAddressV(vkTextureAddressMode addressMode) = 0;
  virtual vkTextureAddressMode GetAddressV() const = 0;

  virtual void SetAddressW(vkTextureAddressMode addressMode) = 0;
  virtual vkTextureAddressMode GetAddressW() const = 0;

  virtual void SetBorderColor(const vkVector4f &color) = 0;
  virtual const vkVector4f &GetBorderColor() const = 0;


};

VK_INTERFACE();
struct VKE_API ITexture : public virtual IObject
{
  VK_CLASS_GEN;

  virtual vkTextureType GetType() const = 0;

  virtual void SetSampler(ISampler *sampler) = 0;
  virtual ISampler* GetSampler() = 0;
  virtual const ISampler* GetSampler() const = 0;

};

VK_INTERFACE();
struct VKE_API ITexture2D : public ITexture
{
  VK_CLASS_GEN;

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;

  virtual void CopyData(vkUInt8 layer, vkSize size, const void *data) = 0;

};
#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Math/Vector.hh>

#include <Valkyrie/Graphics/isampler.refl.hh>

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

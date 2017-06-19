#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/csenums.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/math/csvector.hh>

#include <valkyrie/graphics/isampler.refl.hh>

CS_INTERFACE();
struct CSE_API iSampler : public iObject
{
  CS_CLASS_GEN;


  virtual void SetFilter(csFilterMode filter) = 0;
  virtual csFilterMode GetFilter() const = 0;

  virtual void SetAnisotropy(csUInt8 anisotropy) = 0;
  virtual csUInt8 GetAnisotropy() const = 0;

  virtual void SetMinLOD(csInt16 minLOD) = 0;
  virtual csInt16 GetMinLOD() const = 0;

  virtual void SetMaxLOD(csInt16 minLOD) = 0;
  virtual csInt16 GetMaxLOD() const = 0;

  virtual void SetAddressU(csTextureAddressMode addressMode) = 0;
  virtual csTextureAddressMode GetAddressU() const = 0;

  virtual void SetAddressV(csTextureAddressMode addressMode) = 0;
  virtual csTextureAddressMode GetAddressV() const = 0;

  virtual void SetAddressW(csTextureAddressMode addressMode) = 0;
  virtual csTextureAddressMode GetAddressW() const = 0;

  virtual void SetBorderColor(const csVector4f &color) = 0;
  virtual const csVector4f &GetBorderColor() const = 0;

  virtual void SetTextureCompareMode(csTextureCompareMode mode) = 0;
  virtual csTextureCompareMode GetTextureCompareMode() const = 0;

  virtual void SetTextureCompareFunc(csTextureCompareFunc func) = 0;
  virtual csTextureCompareFunc GetTextureCompareFunc() const = 0;

  virtual bool NeedsMipMaps() const = 0;

};

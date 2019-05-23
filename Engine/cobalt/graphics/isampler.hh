#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/cstypes.hh>

#include <cobalt/graphics/isampler.refl.hh>

namespace cs
{

CS_CLASS();
struct CSE_API iSampler : public cs::iObject
{
  virtual ~iSampler() { }
  CS_CLASS_GEN;


  virtual void SetFilter(cs::eFilterMode filter) = 0;
  virtual cs::eFilterMode GetFilter() const = 0;

  virtual void SetAnisotropy(csUInt8 anisotropy) = 0;
  virtual csUInt8 GetAnisotropy() const = 0;

  virtual void SetMinLOD(csInt16 minLOD) = 0;
  virtual csInt16 GetMinLOD() const = 0;

  virtual void SetMaxLOD(csInt16 minLOD) = 0;
  virtual csInt16 GetMaxLOD() const = 0;

  virtual void SetAddressU(cs::eTextureAddressMode addressMode) = 0;
  virtual cs::eTextureAddressMode GetAddressU() const = 0;

  virtual void SetAddressV(cs::eTextureAddressMode addressMode) = 0;
  virtual cs::eTextureAddressMode GetAddressV() const = 0;

  virtual void SetAddressW(cs::eTextureAddressMode addressMode) = 0;
  virtual cs::eTextureAddressMode GetAddressW() const = 0;

  virtual void SetBorderColor(const cs::Vector4f& color) = 0;
  virtual const cs::Vector4f& GetBorderColor() const = 0;

  virtual void SetTextureCompareMode(cs::eTextureCompareMode mode) = 0;
  virtual cs::eTextureCompareMode GetTextureCompareMode() const = 0;

  virtual void SetTextureCompareFunc(cs::eTextureCompareFunc func) = 0;
  virtual cs::eTextureCompareFunc GetTextureCompareFunc() const = 0;

  virtual bool NeedsMipMaps() const = 0;

};

CS_CLASS()
class CSE_API SamplerWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::iSampler, SamplerWrapper, cs::ResourceWrapper);
};



}

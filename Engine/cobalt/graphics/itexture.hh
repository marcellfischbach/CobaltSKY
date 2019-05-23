#pragma once

#pragma warning(disable: 4250)


#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/graphics/itexture.refl.hh>


namespace cs
{
class SamplerWrapper;

CS_CLASS();
struct CSE_API iTexture : public cs::iObject
{
  CS_CLASS_GEN;
  iTexture() : cs::iObject() { }
  virtual ~iTexture() { }

  virtual cs::eTextureType GetType() const = 0;

  virtual void SetSampler(cs::SamplerWrapper *sampler) = 0;
  virtual cs::SamplerWrapper* GetSampler() = 0;
  virtual const cs::SamplerWrapper* GetSampler() const = 0;

  virtual void GenerateMipMaps() = 0;

};


CS_CLASS()
class CSE_API TextureWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::iTexture, TextureWrapper, cs::ResourceWrapper);
};


}

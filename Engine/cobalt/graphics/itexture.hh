#pragma once

#pragma warning(disable: 4250)


#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/graphics/itexture.refl.hh>

class csSamplerWrapper;

CS_CLASS();
struct CSE_API iTexture : public cs::iObject
{
  CS_CLASS_GEN;
  iTexture() : cs::iObject() { }
  virtual ~iTexture() { }

  virtual csTextureType GetType() const = 0;

  virtual void SetSampler(csSamplerWrapper *sampler) = 0;
  virtual csSamplerWrapper* GetSampler() = 0;
  virtual const csSamplerWrapper* GetSampler() const = 0;

  virtual void GenerateMipMaps() = 0;

};


CS_CLASS()
class CSE_API csTextureWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(iTexture, csTextureWrapper, csResourceWrapper);
};


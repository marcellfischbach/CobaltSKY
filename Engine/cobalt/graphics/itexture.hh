#pragma once

#pragma warning(disable: 4250)


#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/graphics/itexture.refl.hh>

class csSamplerWrapper;

CS_INTERFACE();
struct CSE_API iTexture : public iObject
{
  CS_CLASS_GEN;
  iTexture() : iObject() { }
  virtual ~iTexture() { }

  virtual csTextureType GetType() const = 0;

  virtual void SetSampler(csSamplerWrapper *sampler) = 0;
  virtual csSamplerWrapper* GetSampler() = 0;
  virtual const csSamplerWrapper* GetSampler() const = 0;

  virtual void GenerateMipMaps() = 0;

};

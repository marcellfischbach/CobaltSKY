#pragma once

#pragma warning(disable: 4250)


#include <valkyrie/csexport.hh>
#include <valkyrie/csenums.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/math/csvector.hh>
#include <valkyrie/graphics/itexture.refl.hh>

struct iSampler;

CS_INTERFACE();
struct CSE_API iTexture : public iObject
{
  CS_CLASS_GEN;
  iTexture() : iObject() { }
  virtual ~iTexture() { }

  virtual csTextureType GetType() const = 0;

  virtual void SetSampler(iSampler *sampler) = 0;
  virtual iSampler* GetSampler() = 0;
  virtual const iSampler* GetSampler() const = 0;

  virtual void GenerateMipMaps() = 0;

};

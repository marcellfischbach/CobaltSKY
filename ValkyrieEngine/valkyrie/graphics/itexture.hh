#pragma once

#pragma warning(disable: 4250)


#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/math/vkvector.hh>
#include <valkyrie/graphics/itexture.refl.hh>

struct iSampler;

VK_INTERFACE();
struct VKE_API iTexture : public iObject
{
  VK_CLASS_GEN;
  iTexture() : iObject() { }
  virtual ~iTexture() { }

  virtual vkTextureType GetType() const = 0;

  virtual void SetSampler(iSampler *sampler) = 0;
  virtual iSampler* GetSampler() = 0;
  virtual const iSampler* GetSampler() const = 0;

  virtual void GenerateMipMaps() = 0;

};

#pragma once

#pragma warning(disable: 4250)


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/graphics/itexture.refl.hh>

struct ISampler;

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

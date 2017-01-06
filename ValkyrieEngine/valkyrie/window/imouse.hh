#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/window/imouse.refl.hh>

VK_INTERFACE()
struct VKE_API IMouse : public IObject
{
  VK_CLASS_GEN;

  virtual bool IsButtonDown(vkButton button) const = 0;
  virtual bool IsButtonUp(vkButton button) const = 0;

  virtual bool IsButtonPressed(vkButton button) const = 0;
  virtual bool IsButtonReleased(vkButton button) const = 0;

  virtual vkInt32 GetPosX() const = 0;
  virtual vkInt32 GetPosY() const = 0;

  virtual vkInt32 GetRelX() const = 0;
  virtual vkInt32 GetRelY() const = 0;
};

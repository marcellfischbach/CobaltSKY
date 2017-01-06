#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/window/ikeyboard.refl.hh>

VK_INTERFACE()
struct VKE_API IKeyboard : public IObject
{
  VK_CLASS_GEN;

  virtual bool IsKeyDown(vkKey key) const = 0;
  virtual bool IsKeyUp(vkKey key) const = 0;

  virtual bool IsKeyPressed(vkKey key) const = 0;
  virtual bool IsKeyReleased(vkKey key) const = 0;

};


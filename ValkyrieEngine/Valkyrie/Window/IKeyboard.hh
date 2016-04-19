#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Window/IKeyboard.refl.hh>

VK_INTERFACE()
struct VKE_API IKeyboard : public IObject
{
  VK_CLASS_GEN;

  virtual bool IsKeyDown(vkKey key) const = 0;
  virtual bool IsKeyUp(vkKey key) const = 0;

  virtual bool IsKeyPressed(vkKey key) const = 0;
  virtual bool IsKeyReleased(vkKey key) const = 0;

};


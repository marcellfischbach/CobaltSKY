#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Types.hh>
#include <Valkyrie/Window/IWindow.refl.hh>

struct IKeyboard;
struct IMouse;
VK_INTERFACE()
struct VKE_API IWindow : public IObject
{
  VK_CLASS_GEN;

  virtual const IKeyboard *GetKeyboard() const = 0;
  virtual const IMouse *GetMouse() const = 0;

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;

  virtual vkInt16 GetPosX() const = 0;
  virtual vkInt16 GetPosY() const = 0;

  virtual bool UpdateEvents() = 0;
  virtual bool Present() = 0;

  virtual vkUInt32 GetTicks() const = 0;
};




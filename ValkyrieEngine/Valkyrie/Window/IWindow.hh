#pragma once

#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Types.hh>


VK_INTERFACE()
struct IWindow : public iObject
{
  VK_CLASS_GEN;

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;

  virtual vkInt16 GetPosX() const = 0;
  virtual vkInt16 GetPosY() const = 0;

};




#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/csenums.hh>
#include <cobalt/window/ikeyboard.refl.hh>

namespace cs
{

CS_CLASS()
struct CSE_API iKeyboard : public cs::iObject
{
  CS_CLASS_GEN;

  virtual ~iKeyboard() { }

  virtual bool IsKeyDown(cs::eKey key) const = 0;
  virtual bool IsKeyUp(cs::eKey key) const = 0;

  virtual bool IsKeyPressed(cs::eKey key) const = 0;
  virtual bool IsKeyReleased(cs::eKey key) const = 0;

};


}


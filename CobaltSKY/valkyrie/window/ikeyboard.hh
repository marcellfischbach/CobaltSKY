#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/csenums.hh>
#include <valkyrie/window/ikeyboard.refl.hh>

CS_INTERFACE()
struct CSE_API iKeyboard : public iObject
{
  CS_CLASS_GEN;

  virtual bool IsKeyDown(csKey key) const = 0;
  virtual bool IsKeyUp(csKey key) const = 0;

  virtual bool IsKeyPressed(csKey key) const = 0;
  virtual bool IsKeyReleased(csKey key) const = 0;

};


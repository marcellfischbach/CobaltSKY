#pragma once

#include <windowsdl/sdlexport.hh>
#include <valkyrie/window/ikeyboard.hh>
#include <windowsdl/sdlkeyboard.refl.hh>

CS_CLASS()
class CSWINDOWSDL_API SDLKeyboard : public CS_SUPER(iKeyboard)
{
  CS_CLASS_GEN_OBJECT;
public:

  SDLKeyboard();
  virtual ~SDLKeyboard();

  void UpdateKeys();
  void SetKeyDown(csUInt32 key);
  void SetKeyUp(csUInt32 key);

  bool IsKeyDown(csKey key) const;
  bool IsKeyUp(csKey key) const;

  bool IsKeyPressed(csKey key) const;
  bool IsKeyReleased(csKey key) const;

private:
  bool m_current[eK_COUNT];
  bool m_prev[eK_COUNT];
};

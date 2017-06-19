#pragma once

#include <windowsdl/sdlexport.hh>
#include <valkyrie/window/imouse.hh>
#include <windowsdl/sdlmouse.refl.hh>

CS_CLASS()
class CSWINDOWSDL_API SDLMouse : public CS_SUPER(iMouse)
{
  CS_CLASS_GEN_OBJECT;
public:

  SDLMouse();
  virtual ~SDLMouse();

  void UpdateButtons();
  void SetButtonDown(csUInt8 key);
  void SetButtonUp(csUInt8 key);

  bool IsButtonDown(csButton button) const;
  bool IsButtonUp(csButton button) const;

  bool IsButtonPressed(csButton button) const;
  bool IsButtonReleased(csButton button) const;

  virtual csInt32 GetPosX() const;
  virtual csInt32 GetPosY() const;

  virtual csInt32 GetRelX() const;
  virtual csInt32 GetRelY() const;


  void UpdateMotion();
  void SetMotion(csInt32 x, csInt32 y, csInt32 relX, csInt32 relY);

private:
  bool m_current[eB_COUNT];
  bool m_prev[eB_COUNT];

  csInt32 m_posX;
  csInt32 m_posY;
  csInt32 m_relX;
  csInt32 m_relY;
};

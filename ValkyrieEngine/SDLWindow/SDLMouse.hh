#pragma once

#include <SDLWindow/Export.hh>
#include <Valkyrie/Window/IMouse.hh>
#include <SDLWindow/SDLMouse.refl.hh>

VK_CLASS()
class SDLMouse : public VK_SUPER(IMouse)
{
  VK_CLASS_GEN_OBJECT;
public:

  SDLMouse();
  virtual ~SDLMouse();

  void UpdateButtons();
  void SetButtonDown(vkUInt8 key);
  void SetButtonUp(vkUInt8 key);

  bool IsButtonDown(vkButton button) const;
  bool IsButtonUp(vkButton button) const;

  bool IsButtonPressed(vkButton button) const;
  bool IsButtonReleased(vkButton button) const;

  virtual vkInt32 GetPosX() const;
  virtual vkInt32 GetPosY() const;

  virtual vkInt32 GetRelX() const;
  virtual vkInt32 GetRelY() const;


  void UpdateMotion();
  void SetMotion(vkInt32 x, vkInt32 y, vkInt32 relX, vkInt32 relY);

private:
  bool m_current[eB_COUNT];
  bool m_prev[eB_COUNT];

  vkInt32 m_posX;
  vkInt32 m_posY;
  vkInt32 m_relX;
  vkInt32 m_relY;
};

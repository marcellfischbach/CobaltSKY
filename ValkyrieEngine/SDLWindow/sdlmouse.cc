
#include <SDLWindow/sdlmouse.hh>
#include <SDL.h>

static vkButton buttonMap[32];

SDLMouse::SDLMouse()
{
  VK_CLASS_GEN_CONSTR;

  memset(m_current, 0, sizeof(m_current));
  memset(m_prev, 0, sizeof(m_prev));
  memset(buttonMap, 0, sizeof(buttonMap));
  m_posX = m_posY = m_relX = m_relY = 0;
#include <SDLWindow/sdlbuttonmap.hh>
}

SDLMouse::~SDLMouse()
{
}

void SDLMouse::UpdateButtons()
{
  memcpy(m_prev, m_current, sizeof(m_current));
}

void SDLMouse::SetButtonDown(vkUInt8 key)
{
  if (key < 32)
  {
    vkButton b = buttonMap[key];
    m_current[b] = true;
  }

}

void SDLMouse::SetButtonUp(vkUInt8 key)
{
  if (key < 32)
  {
    vkButton b = buttonMap[key];
    m_current[b] = false;
  }
}

bool SDLMouse::IsButtonDown(vkButton button) const
{
  return m_current[button];
}

bool SDLMouse::IsButtonUp(vkButton button) const
{
  return !m_current[button];
}

bool SDLMouse::IsButtonPressed(vkButton button) const
{
  return m_current[button] && !m_prev[button];
}

bool SDLMouse::IsButtonReleased(vkButton button) const
{
  return !m_current[button] && m_prev[button];
}

void SDLMouse::UpdateMotion()
{
  m_relX = m_relY = 0;
}

void SDLMouse::SetMotion(vkInt32 x, vkInt32 y, vkInt32 relX, vkInt32 relY)
{
  m_posX = x;
  m_posY = y;
  m_relX = relX;
  m_relY = relY;
}

vkInt32 SDLMouse::GetPosX() const
{
  return m_posX;
}

vkInt32 SDLMouse::GetPosY() const
{
  return m_posY;
}

vkInt32 SDLMouse::GetRelX() const
{
  return m_relX;
}

vkInt32 SDLMouse::GetRelY() const
{
  return m_relY;
}


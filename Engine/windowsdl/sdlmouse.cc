
#include <windowsdl/sdlmouse.hh>
#include <SDL.h>

static cs::eButton buttonMap[32];

SDLMouse::SDLMouse()
{
  CS_CLASS_GEN_CONSTR;

  memset(m_current, 0, sizeof(m_current));
  memset(m_prev, 0, sizeof(m_prev));
  memset(buttonMap, 0, sizeof(buttonMap));
  m_posX = m_posY = m_relX = m_relY = 0;
#include <windowsdl/sdlbuttonmap.hh>
}

SDLMouse::~SDLMouse()
{
}

void SDLMouse::UpdateButtons()
{
  memcpy(m_prev, m_current, sizeof(m_current));
}

void SDLMouse::SetButtonDown(csUInt8 key)
{
  if (key < 32)
  {
    cs::eButton b = buttonMap[key];
    m_current[b] = true;
  }

}

void SDLMouse::SetButtonUp(csUInt8 key)
{
  if (key < 32)
  {
    cs::eButton b = buttonMap[key];
    m_current[b] = false;
  }
}

bool SDLMouse::IsButtonDown(cs::eButton button) const
{
  return m_current[button];
}

bool SDLMouse::IsButtonUp(cs::eButton button) const
{
  return !m_current[button];
}

bool SDLMouse::IsButtonPressed(cs::eButton button) const
{
  return m_current[button] && !m_prev[button];
}

bool SDLMouse::IsButtonReleased(cs::eButton button) const
{
  return !m_current[button] && m_prev[button];
}

void SDLMouse::UpdateMotion()
{
  m_relX = m_relY = 0;
}

void SDLMouse::SetMotion(csInt32 x, csInt32 y, csInt32 relX, csInt32 relY)
{
  m_posX = x;
  m_posY = y;
  m_relX = relX;
  m_relY = relY;
}

csInt32 SDLMouse::GetPosX() const
{
  return m_posX;
}

csInt32 SDLMouse::GetPosY() const
{
  return m_posY;
}

csInt32 SDLMouse::GetRelX() const
{
  return m_relX;
}

csInt32 SDLMouse::GetRelY() const
{
  return m_relY;
}



#pragma once

#include <windowsdl/sdlexport.hh>
#include <cobalt/window/iwindow.hh>
#include <SDL2/SDL.h>
#include <string>
#include <windowsdl/sdlwindow.refl.hh>

class SDLKeyboard;
class SDLMouse;

CS_CLASS()
class CSWINDOWSDL_API SDLWindow : public CS_SUPER(iWindow)
{
  CS_CLASS_GEN_OBJECT;
public:

  SDLWindow();
  virtual ~SDLWindow();

  bool InitializeOpenGL(
    const std::string &title,
    csUInt16 width,
    csUInt16 height,
    csInt16 posX,
    csInt16 posY,
    bool fullscreen,
    csUInt8 openGLMajor,
    csUInt8 openGLMinor);

  virtual void SetIcon(const csImage *image);

  virtual const iKeyboard *GetKeyboard() const;
  virtual const iMouse *GetMouse() const;

  virtual csUInt16 GetWidth() const;
  virtual csUInt16 GetHeight() const;

  virtual csInt16 GetPosX() const;
  virtual csInt16 GetPosY() const;

  virtual csUInt32 GetTicks() const;

  virtual bool UpdateEvents();
  virtual bool Present();
private:
  csUInt16 m_width;
  csUInt16 m_height;

  csInt16 m_posX;
  csInt16 m_posY;

  SDL_Window *m_window;
  SDL_GLContext m_context;

  SDLKeyboard *m_keyboard;
  SDLMouse *m_mouse;
};




#include <windowsdl/sdlwindow.hh>
#include <windowsdl/sdlkeyboard.hh>
#include <windowsdl/sdlmouse.hh>
#include <Windows.h>
#include <GL/GL.h>

cs::SDLWindow::SDLWindow()
  : m_window(0)
  , m_keyboard(new cs::SDLKeyboard())
  , m_mouse(new cs::SDLMouse())
{
  CS_CLASS_GEN_CONSTR;
}

cs::SDLWindow::~SDLWindow()
{
  if (m_window)
  {
    SDL_DestroyWindow(m_window);
  }
}

bool cs::SDLWindow::InitializeOpenGL(
  const std::string &title,
  csUInt16 width,
  csUInt16 height,
  csInt16 posX,
  csInt16 posY,
  bool fullscreen,
  csUInt8 openGLMajor,
  csUInt8 openGLMinor)
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);

  int numDrivers = SDL_GetNumVideoDrivers();
  printf("Found %u Video drivers\n", numDrivers);
  for (int i = 0; i < numDrivers; ++i)
  {
	  const char *videoDriver = SDL_GetVideoDriver(i);
	  printf("   %s\n", videoDriver);
  }

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  /*
  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
  */

  
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, openGLMajor);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, openGLMinor);
  
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
  if (fullscreen)
  {
    flags |= SDL_WINDOW_FULLSCREEN;
  }
  m_window = SDL_CreateWindow(title.c_str(),
                              posX, posY,
                              width, height,
                              flags);
  if (!m_window)
  {
    return false;
  }
  m_posX = posX;
  m_posY = posY;
  m_width = width;
  m_height = height;

  m_context = SDL_GL_CreateContext(m_window);

  if (SDL_GL_SetSwapInterval(0))
  {
    printf("Set swap interval: %s\n", SDL_GetError());
  }

  SDL_SetRelativeMouseMode(SDL_TRUE);
  glViewport(0, 0, width, height);


  return true;

}

void cs::SDLWindow::SetIcon(const cs::Image *image)
{
  csUInt8 *buffer = new csUInt8[image->GetWidth() * image->GetHeight() * 4];
  memcpy(buffer, image->GetData(), image->GetWidth() * image->GetHeight() * 4);
  SDL_Surface *iconSurface = SDL_CreateRGBSurfaceFrom(buffer, 
                                                      image->GetWidth(), 
                                                      image->GetHeight(), 
                                                      32, 
                                                      image->GetWidth() * 4,
                                                      0x000000ff, 
                                                      0x0000ff00, 
                                                      0x00ff0000,
                                                      0xff000000);
  SDL_SetWindowIcon(m_window, iconSurface);
  SDL_FreeSurface(iconSurface);
  delete[] buffer;

}

const cs::iKeyboard *cs::SDLWindow::GetKeyboard() const
{
  return m_keyboard;
}


const cs::iMouse *cs::SDLWindow::GetMouse() const
{
  return m_mouse;
}

bool cs::SDLWindow::UpdateEvents()
{
  if (!m_window)
  {
    printf("no window\n");
    return false;
  }
  SDL_GL_MakeCurrent(m_window, m_context);

  m_keyboard->UpdateKeys();
  m_mouse->UpdateButtons();
  m_mouse->UpdateMotion();

  SDL_Event evt;
  while (SDL_PollEvent(&evt))
  {

    // do nothing with the event for now.
    switch (evt.type)
    {
    case SDL_KEYDOWN:
      m_keyboard->SetKeyDown(evt.key.keysym.scancode);
      break;
    case SDL_KEYUP:
      m_keyboard->SetKeyUp(evt.key.keysym.scancode);
      break;

    case SDL_MOUSEBUTTONDOWN:
      m_mouse->SetButtonDown(evt.button.button);
      break;
    case SDL_MOUSEBUTTONUP:
      m_mouse->SetButtonUp(evt.button.button);
      break;

    case SDL_MOUSEMOTION:
      m_mouse->SetMotion(evt.motion.x, evt.motion.y, evt.motion.xrel, evt.motion.yrel);
      break;
    }
  }
  return true;
}

bool cs::SDLWindow::Present()
{

  SDL_GL_MakeCurrent(m_window, m_context);
  SDL_GL_SwapWindow(m_window);
  return true;
}

csUInt16 cs::SDLWindow::GetWidth() const
{
  return m_width;
}

csUInt16 cs::SDLWindow::GetHeight() const
{
  return m_height;
}

csInt16 cs::SDLWindow::GetPosX() const
{
  return m_posX;
}

csInt16 cs::SDLWindow::GetPosY() const
{
  return m_posY;
}

csUInt32 cs::SDLWindow::GetTicks() const
{
  return SDL_GetTicks();
}

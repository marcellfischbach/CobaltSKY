#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Window/IWindow.hh>

class VKE_API vkEngine
{
public:
  static vkEngine *Get();

  void Set
private:
  vkEngine();

  IWindow *m_window;  
};

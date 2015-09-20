

#include <Valkyrie/Engine.hh>

vkEngine::vkEngine()
{

}

vkEngine *vkEngine::Get()
{
  static vkEngine engine;
  return &engine;
}



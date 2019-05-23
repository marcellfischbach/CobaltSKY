
#include <editor/eventbus.hh>

EventBus::EventBus()
  : cs::EventBus()
{

}

EventBus &EventBus::Get()
{
  static EventBus eventBus;
  return eventBus;
}


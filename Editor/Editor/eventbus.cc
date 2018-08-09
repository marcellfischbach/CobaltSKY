
#include <eventbus.hh>

EventBus::EventBus()
  : csEventBus()
{

}

EventBus &EventBus::Get()
{
  static EventBus eventBus;
  return eventBus;
}


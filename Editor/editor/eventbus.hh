#pragma once

#include <editor/editorexport.hh>
#include <cobalt/core/cseventbus.hh>

class EventBus : public csEventBus
{
public:
  static EventBus &Get();
private:
  EventBus();

};
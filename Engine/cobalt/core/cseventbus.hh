
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <csrefl/class.hh>
#include <string>
#include <vector>
#include <map>

namespace cs
{
class Event;

typedef  void(*EventDelegate)(cs::Event&, void*);

struct iEventHandler
{
  virtual ~iEventHandler() {}
  virtual void HandleEvent(cs::Event& event) = 0;
};

class CSE_API EventBus
{
public:
  EventBus();
  ~EventBus();

public:
  void Fire(cs::Event& event);
  cs::EventBus& operator <<(cs::Event& event);



  void Register(cs::EventDelegate delegate, void* userObject = 0);
  void Register(const cs::Class* cls, cs::EventDelegate delegate, void* userObject = 0);
  void Deregister(cs::EventDelegate delegate);
  void Deregister(cs::EventDelegate delegate, void* userObject);

  void Register(iEventHandler* handler);
  void Register(const cs::Class* cls, iEventHandler* handler);
  void Deregister(iEventHandler* handler);

private:
  void Fire(cs::Event& event, const cs::Class* cls);

  struct Delegate
  {
    cs::EventDelegate delegate;
    void* ptr;
  };

  std::map<const cs::Class*, std::vector<iEventHandler*>> m_handlers;
  std::map<const cs::Class*, std::vector<Delegate>> m_delegates;
};

}


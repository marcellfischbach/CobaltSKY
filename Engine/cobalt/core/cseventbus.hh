
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <string>
#include <vector>
#include <map>

class csEvent;

typedef  void(*csEventDelegate)(csEvent &, void*);

struct iEventHandler 
{
  virtual ~iEventHandler() {}
  virtual void HandleEvent(csEvent &event) = 0;
};

class CSE_API csEventBus 
{
public:
  csEventBus();
  ~csEventBus();

public:
  void Fire(csEvent &event);
  csEventBus &operator <<(csEvent &event);



  void Register(csEventDelegate delegate, void *userObject = 0);
  void Register(const csClass *cls, csEventDelegate delegate, void *userObject = 0);
  void Deregister(csEventDelegate delegate);
  void Deregister(csEventDelegate delegate, void *userObject);

  void Register(iEventHandler *handler);
  void Register(const csClass *cls, iEventHandler *handler);
  void Deregister(iEventHandler *handler);

private:
  void Fire(csEvent &event, const csClass *cls);

  struct Delegate
  {
    csEventDelegate delegate;
    void *ptr;
  };
  
  std::map<const csClass *, std::vector<iEventHandler *>> m_handlers;
  std::map<const csClass *, std::vector<Delegate>> m_delegates;
};


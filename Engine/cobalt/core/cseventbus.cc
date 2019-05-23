

#include <cobalt/core/cseventbus.hh>
#include <cobalt/core/csevent.hh>


cs::EventBus::EventBus()
{

}

cs::EventBus::~EventBus()
{

}

void cs::EventBus::Fire(cs::Event &event)
{
  Fire(event, event.GetClass());
}

void cs::EventBus::Fire(cs::Event &event, const cs::Class *cls)
{
  if (!cls || cls == cs::Object::GetStaticClass())
  {
    return;
  }

  std::map<const cs::Class *, std::vector<Delegate>>::iterator it = m_delegates.find(cls);
  if (it != m_delegates.end())
  {
    for (auto del : it->second)
    {
      del.delegate(event, del.ptr);
    }
  }

  std::map<const cs::Class *, std::vector<iEventHandler*>>::iterator it1 = m_handlers.find(cls);
  if (it1 != m_handlers.end())
  {
    for (auto handler : it1->second)
    {
      handler->HandleEvent(event);
    }
  }

  for (csSize i = 0, in = cls->GetNumberOfSuperClasses(); i < in; ++i)
  {
    Fire(event, cls->GetSuperClass(i));
  }
}

cs::EventBus &cs::EventBus::operator<<(cs::Event &event)
{
  Fire(event);
  return *this;
}


void cs::EventBus::Register(cs::EventDelegate delegate, void *ptr)
{
  Register(cs::Event::GetStaticClass(), delegate, ptr);
}

void cs::EventBus::Register(const cs::Class *eventClass, cs::EventDelegate delegate, void *ptr)
{
  Delegate del;
  del.delegate = delegate;
  del.ptr = ptr;
  m_delegates[eventClass].push_back(del);
}

void cs::EventBus::Deregister(cs::EventDelegate delegate, void *ptr)
{
  
  for (auto dels: m_delegates)
  {
    for (std::vector<Delegate>::iterator it = dels.second.begin();
      it != dels.second.end(); )
    {
      if (it->delegate == delegate && it->ptr == ptr)
      {
        it = dels.second.erase(it);
      }
      else
      {
        it++;
      }
    }
  }
}


void cs::EventBus::Deregister(cs::EventDelegate delegate)
{

  for (auto dels : m_delegates)
  {
    for (std::vector<Delegate>::iterator it = dels.second.begin();
      it != dels.second.end(); )
    {
      if (it->delegate == delegate)
      {
        it = dels.second.erase(it);
      }
      else
      {
        it++;
      }
    }
  }
}




void cs::EventBus::Register(iEventHandler *handler)
{
  Register(cs::Event::GetStaticClass(), handler);
}

void cs::EventBus::Register(const cs::Class *eventClass, iEventHandler *handler)
{
  m_handlers[eventClass].push_back(handler);
}

void cs::EventBus::Deregister(iEventHandler *handler)
{

  for (auto hndl : m_handlers)
  {
    for (std::vector<iEventHandler *>::iterator it = hndl.second.begin();
      it != hndl.second.end(); )
    {
      if (*it == handler)
      {
        it = hndl.second.erase(it);
      }
      else
      {
        it++;
      }
    }
  }
}


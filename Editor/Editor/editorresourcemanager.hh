#pragma once


#include <cobalt/core/csresourcemanager.hh>

class EditorResourceManager : public csResourceManager
{
public:
  EditorResourceManager();
  virtual ~EditorResourceManager();

  virtual iObject *Load(const csResourceLocator &locator, iObject *userData = 0);
  virtual const csClass *EvalClass(const csResourceLocator &locator, iObject *userData = 0) const;


  virtual bool RegisterObject(const csResourceLocator &locator, iObject *object);
  virtual void RenameResource(const csResourceLocator &from, const csResourceLocator &to);

private:
  csResourceLocator FixResourceLocator(const csResourceLocator &locator) const;
  bool IsAnonymousLocator(const csResourceLocator &locator) const;

};
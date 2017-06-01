#pragma once


#include <valkyrie/core/vkresourcemanager.hh>

class EditorResourceManager : public vkResourceManager
{
public:
  EditorResourceManager();
  virtual ~EditorResourceManager();

  virtual iObject *Load(const vkResourceLocator &locator, iObject *userData = 0);
  virtual const vkClass *EvalClass(const vkResourceLocator &locator, iObject *userData = 0) const;


  virtual bool RegisterObject(const vkResourceLocator &locator, iObject *object);

private:
  vkResourceLocator FixResourceLocator(const vkResourceLocator &locator) const;
  bool IsAnonymousLocator(const vkResourceLocator &locator) const;

};
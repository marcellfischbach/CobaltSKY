#pragma once


#include <editor/editorexport.hh>
#include <cobalt/core/csresourcemanager.hh>

class EditorResourceManager : public csResourceManager
{
public:
  EditorResourceManager();
  virtual ~EditorResourceManager();

  virtual csResourceWrapper *Load(const csResourceLocator &locator);
  virtual const csClass *EvalClass(const csResourceLocator &locator) const;


  virtual bool RegisterObject(const csResourceLocator &locator, csResourceWrapper *object);
  virtual void RenameResource(const csResourceLocator &from, const csResourceLocator &to);

private:
  csResourceLocator FixResourceLocator(const csResourceLocator &locator) const;
  bool IsAnonymousLocator(const csResourceLocator &locator) const;

};

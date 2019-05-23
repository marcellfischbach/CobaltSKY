#pragma once


#include <editor/editorexport.hh>
#include <cobalt/core/csresourcemanager.hh>

class EditorResourceManager : public cs::ResourceManager
{
public:
  EditorResourceManager();
  virtual ~EditorResourceManager();

  virtual cs::ResourceWrapper *Load(const cs::ResourceLocator &locator);
  virtual const cs::Class *EvalClass(const cs::ResourceLocator &locator) const;


  virtual bool RegisterObject(const cs::ResourceLocator &locator, cs::ResourceWrapper *object);
  virtual void RenameResource(const cs::ResourceLocator &from, const cs::ResourceLocator &to);

private:
  cs::ResourceLocator FixResourceLocator(const cs::ResourceLocator &locator) const;
  bool IsAnonymousLocator(const cs::ResourceLocator &locator) const;

};

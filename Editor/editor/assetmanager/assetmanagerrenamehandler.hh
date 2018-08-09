#pragma once


#include <editor/editorexport.hh>
#include <cobalt/core/cseventbus.hh>

class QDomDocument;
class QDomElement;
class csResourceLocator;
class ProjectAssetReference;
class AssetRenamedEvent;
class AssetManagerRenameHandler : public iEventHandler 
{
public:
  AssetManagerRenameHandler() { };
  virtual ~AssetManagerRenameHandler() { }

  virtual void HandleEvent(csEvent &event);

  static void Register();

private:
  void Rename(const csResourceLocator &resource, const csResourceLocator &from, const csResourceLocator &to);
  void Rename(QDomDocument doc, QDomElement element, const csResourceLocator &from, const csResourceLocator &to);
};
